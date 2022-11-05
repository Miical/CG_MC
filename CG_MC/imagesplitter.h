#ifndef IMAGESPLITTER_H
#define IMAGESPLITTER_H
#include <vector>
#include "imageloader.h"

/**
* 图片分割
* 将图片划分为指定网格大小的网格图，返回其中一个或多个网格拼接所代表的图像。
*/
class ImageSplitter {
public:
	int columnCount()const {
		return originImg->width / gridWidth;
	}
	int rowCount()const {
		return originImg->height/ gridHeight;
	}
	int convertToID(int row, int column)const {
		return row * columnCount() + column;
	}
	ImageSplitter(const Image* originImg_, int gridHeight_, int gridWidth_);
	Image* getImage(int row, int column)const;
	Image* getImage(int id)const;
	Image* getImage(const std::vector<int>& id, bool horizontal)const;
private:
	const Image* originImg; 
	int gridHeight, gridWidth;
};

#endif
