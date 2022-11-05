#include "imagesplitter.h"

ImageSplitter::ImageSplitter(
	Image* originImg_, int gridHeight_, int gridWidth_): 
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
	return new Image(pixels, gridWidth, gridHeight);
}

Image* ImageSplitter::getImage(int id)const {
	return getImage(id / columnCount(), id % columnCount());
}
#include <iostream>
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
	if (horizontal)
		return new Image(pixels, gridWidth * id.size(), gridHeight); 
	else 
		return new Image(pixels, gridWidth, gridHeight * id.size());
}
