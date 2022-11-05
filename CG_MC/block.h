#ifndef BLOCK_H
#define BLOCK_H
#include <GL/glut.h>
#include <cstring>

class BlockBase {
public:
	/**
	* 在位置(x, y, z)渲染该方块
	* @param (x, y, z) 方块所在位置
	* @param direct 方框六种朝向 
	* @param rotate 四种旋转方向
	*/
	void render(float x, float y, float z, int direct, int rotate)const;
	BlockBase(
		int heightNum_, int widthNum_,
		const float interval_[6], 
		const int texture_[6], const int textureDirect_[6]);
private:
	int heightNum, widthNum; // 高度、宽度所占格子数量
	float interval[6]; // 前后上下左右距离边缘的空隙大小
	int texture[6]; // 六个面的材质 前后上下左右
	int textureDirect[6]; // 六个面材质的四种旋转方向 
};

class Block : public BlockBase {
public:
	Block(const int texture_[6], const int textureDirect_[6]);
};


#endif
