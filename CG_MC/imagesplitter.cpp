#include "imagesplitter.h"

ImageSplitter::ImageSplitter(
	const Image* originImg_, int gridHeight_, int gridWidth_): 
		originImg(originImg_), gridHeight(gridHeight_), gridWidth(gridWidth_) {} 


Image* ImageSplitter::getImage(int row, int column)const {
	char* pixels = new char [gridHeight * gridWidth * 3];
	int cnt = 0;
	for (int i = row * gridHeight; i < (row + 1) * gridHeight; i++) {
		for (int j = column * gridWidth; j < (column + 1) * gridWidth; j++) {
			int id = i * originImg->width + j;
			for (int c = 0; c < 3; c++) {
				pixels[cnt++] = originImg->pixels[id * 3 + c];
			}
		}
	}
	Image* img = new Image(pixels, gridWidth, gridHeight);
	convertToRGBA(img);
	return img;
}

Image* ImageSplitter::getImage(int id)const {
	return getImage(id / columnCount(), id % columnCount());
}
Image* ImageSplitter::getImage(const std::vector<int>& id, bool horizontal)const {
	char* pixels = new char[id.size() * gridHeight * gridWidth * 3];
	for (int id_ = 0; id_ < id.size(); id_++) {
		Image* img = getImage(id[id_]);
		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				int newPos;
				if (horizontal)
					newPos = i * (id.size() * gridWidth) + (id_ * gridWidth + j);
				else
					newPos = id_ * gridHeight * gridWidth + (i * gridWidth + j);

				for (int c = 0; c < 3; c++) {
					pixels[newPos * 3 + c] = img->pixels[(i * gridWidth + j) * 3 + c];
				}
			}
		}
		delete img;
	}
	Image* img;
	if (horizontal)
		img = new Image(pixels, gridWidth * id.size(), gridHeight); 
	else 
		img = new Image(pixels, gridWidth, gridHeight * id.size());
	convertToRGBA(img);
	return img;
}

void ImageSplitter::convertToRGBA(Image* img) {
	int pixelsNum = img->height * img->width;
	char* pixels = new char [pixelsNum * 4];
	for (int i = 0; i < pixelsNum; i++) {
		char& R = img->pixels[i * 3],
			& G = img->pixels[i * 3 + 1],
			& B = img->pixels[i * 3 + 2];
		bool isTransparent = (unsigned char(R) == 0xff
			&& unsigned char(G) == 0xff
			&& unsigned char(B) == 0xff);
		pixels[i * 4] = R;
		pixels[i * 4 + 1] = G;
		pixels[i * 4 + 2] = B;
		pixels[i * 4 + 3] = isTransparent ? 0 : 255;
	}
	delete[] img->pixels;
	img->pixels = pixels;
}
