#ifndef BLOCK_H
#define BLOCK_H
#include <GL/glut.h>
#include <cstring>

/// <summary>
/// 方块基类
/// </summary>
class BlockBase {
public:
	virtual void render(float x, float y, float z)const = 0;
	virtual void renderTargetBlock(float x, float y, float z)const;
	virtual bool containPoint(float x, float y, float z)const;
	BlockBase(int lengthNum_, int widthNum_, int heightNum_);
private:
	int lengthNum, widthNum, heightNum; // 高度、宽度所占格子数量
};

/// <summary>
/// 普通方块类。材质不透明，三个方向上长度都为1。
/// </summary>
class Block : public BlockBase {
public:
	Block(const int texture_[6]);
	Block(const int texture_);
	void render(float x, float y, float z)const override;
private:
	/// <summary>
	/// 六个面的材质，顺序为前后左右上下。
	/// </summary>
	int texture[6];
};

/// <summary>
/// 树叶类。材质透明，六个面材质一致。
/// </summary>
class Leaves : public Block {
public:
	Leaves(int texture_);
	void render(float x, float y, float z) const override;
};

/// <summary>
/// 植物类。两张图同材质，材质透明，人物无法碰撞。 
/// </summary>
class Plant : public BlockBase {
public:
	Plant(int texture);
	void render(float x, float y, float z) const override;
	bool containPoint(float x, float y, float z)const override;
private:
	int texture;
};

#endif
