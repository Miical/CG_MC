#ifndef BLOCK_H
#define BLOCK_H
#include <GL/glut.h>
#include <cstring>

class BlockBase {
public:
	/**
	* 在位置(x, y, z)渲染该方块
	* @param (x, y, z) 方块所在位置
	*/
	virtual void render(float x, float y, float z)const = 0;
	virtual void renderTargetBlock(float x, float y, float z)const;
	BlockBase(int lengthNum_, int widthNum_, int heightNum_);
private:
	int lengthNum, widthNum, heightNum; // 高度、宽度所占格子数量
};

class Block : public BlockBase {
public:
	Block(const int texture_[6]);
	void render(float x, float y, float z)const override;
	// void renderHighlightedBlock(float x, float y, float z)const override;
private:
	int texture[6]; // 六个面的材质 前后左右上下
	typedef GLfloat vertex3[3];
	const vertex3 cube[8] = {
		{ -0.5, -0.5, -0.5 }, { -0.5, 0.5, -0.5}, 
		{ -0.5, 0.5, 0.5 }, { -0.5, -0.5, 0.5}, 
		{ 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5}, 
		{ 0.5, 0.5, 0.5 }, { 0.5, -0.5, 0.5}, 
	};
};

#endif
