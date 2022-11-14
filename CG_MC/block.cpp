#include "block.h"
#include "blocktype.h"

/// <summary>
/// BlockBase类构造函数。
/// </summary>
/// <param name="lengthNum_">方块高度所占格子数</param>
/// <param name="widthNum_">方块宽度所占格子数</param>
/// <param name="heightNum_">方块长度所占格子数</param>
/// <param name="filledBlock_">是否为填充类型的方块</param>
BlockBase::BlockBase( int lengthNum_, int widthNum_, int heightNum_, 
	bool filledBlock_) :
	lengthNum(lengthNum_), heightNum(heightNum_), widthNum(widthNum_),
	filledBlock(filledBlock_) {}

/// <summary>
/// 在(x, y, z)位置渲染被准星瞄准的方块。
/// </summary>
/// <param name="mask">渲染面掩码</param>
void BlockBase::renderTargetBlock(float x, float y, float z, unsigned char mask)const {
	render(x, y, z, mask);

	const vertex3f v[8] = {
		{ 0.0f, 0.0f, 0.0f }, { lengthNum, 0.0f, 0.0f },
		{ lengthNum, widthNum, 0.0f },{ 0.0f, widthNum, 0.0f }, 
		{ 0.0f, 0.0f, heightNum }, { lengthNum, 0.0f, heightNum },
		{ lengthNum, widthNum, heightNum }, { 0.0f, widthNum, heightNum }
	};

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(4.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(x, y, z);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++) {
		glVertex3fv(v[i]); glVertex3fv(v[(i + 1) % 4]);
		glVertex3fv(v[i + 4]); glVertex3fv(v[(i + 1) % 4 + 4]);
		glVertex3fv(v[i]); glVertex3fv(v[i + 4]);
	}
	glEnd();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
}

/// <summary>
/// 判断方块内部内部是否含有点(x, y, z)，用于人物碰撞检测。
/// <remarks>传入坐标为相对坐标，坐标值需保证在方块范围内。</remarks>
/// </summary>
/// <returns>方块内部若含有该点则返回true，否则为false</returns>
bool BlockBase::containPoint(float x, float y, float z)const {
	return true;
}

/// <summary>
/// 判断方块对type类型而言是否为填充类型，若是则内部面不会被渲染。
/// </summary>
/// <param name="type'>方块类型</param>
/// <returns>方块若对type类型而言是填充类型则返回true，否则为false</returns>
bool BlockBase::isFilledBlock(int type)const {
	return filledBlock;
}

/// <summary>
/// Block类构造函数。
/// </summary>
/// <param name="texture_">六个面的材质类型，顺序为前后左右上下</param>
/// <param name="filledBlock_">是否为填充类型的方块</param>
Block::Block(const int texture_[6], bool isFilledBlock_) : 
	BlockBase(1, 1, 1, isFilledBlock_) {
	memcpy(texture, texture_, sizeof(texture));
}

/// <summary>
/// Block类构造函数。
/// </summary>
/// <param name="texture_">材质类型，应用于六个面</param>
Block::Block(const int texture_, bool isFilledBlock_) : 
	BlockBase(1, 1, 1, isFilledBlock_) {
	for (int i = 0; i < 6; i++)
		texture[i] = texture_;
}

/// <summary>
/// 在坐标(x, y, z)处渲染该方块。
/// </summary>
/// <param name="mask">渲染面掩码，0-5位分别代表“前后左右上下”六面</param>
void Block::render(float x, float y, float z, unsigned char mask)const {
	if (!mask) return;
	typedef GLfloat vertex3[3];
	const vertex3 cube[8] = {
		{ -0.5, -0.5, -0.5 }, { -0.5, 0.5, -0.5}, 
		{ -0.5, 0.5, 0.5 }, { -0.5, -0.5, 0.5}, 
		{ 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5}, 
		{ 0.5, 0.5, 0.5 }, { 0.5, -0.5, 0.5}, 
	};
	const int vertexsOrder[6][4] = {
		{ 4, 5, 6, 7 }, { 1, 0, 3, 2 }, 
		{ 0, 4, 7, 3 }, { 5, 1, 2, 6 },
		{ 7, 6, 2, 3 }, { 4, 5, 1, 0 }
	};
	const GLfloat normals[6][3] = {
		{ 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }
	};
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(x + 0.5f, y + 0.5f, z + 0.5f);
	for (int i = 0; i < 6; i++) {
		if (!((mask >> i) & 1)) continue;

		glBindTexture(GL_TEXTURE_2D, TEXTURE[texture[i]]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
			glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
			glTexCoord2f(0.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][0]]);
			glTexCoord2f(1.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][1]]);
			glTexCoord2f(1.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][2]]);
			glTexCoord2f(0.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][3]]);
		glEnd();
	}
	glPopMatrix();
}
	

int Block::getThumbnailTexture()const {
	return texture[4];
}

/// <summary>
/// Leaves类构造函数
/// </summary>
/// <param name="texture_">材质类型</param>
Leaves::Leaves(int texture_) : Block(texture_, false) {}

/// <summary>
/// 在坐标(x, y, z)处渲染该方块。
/// </summary>
/// <param name="mask">渲染面掩码, 叶子类型渲染所有面，忽略该掩码</param>
void Leaves::render(float x, float y, float z, unsigned char mask)const {
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	Block::render(x, y, z, (1u << 6) - 1u);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

int Plant::getThumbnailTexture()const {
	return texture;
}

/// <summary>
/// Plant类构造函数
/// </summary>
/// <param name="texture_">材质类型</param>
Plant::Plant(int texture_) : texture(texture_), BlockBase(1, 1, 1, false) {}

/// <summary>
/// 在坐标(x, y, z)处渲染该方块。
/// </summary>
/// <param name="mask">渲染面掩码，植物类型渲染所有面，忽略该掩码</param>
void Plant::render(float x, float y, float z, unsigned char mask)const {
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	const vertex3f cube[2][4] = {
		{{ 0.5, 0, -0.5 }, { -0.5, 0, -0.5 }, { -0.5, 0, 0.5}, { 0.5, 0, 0.5}},
		{{ 0, -0.5, -0.5 }, { 0, 0.5, -0.5}, { 0, 0.5, 0.5 }, { 0, -0.5, 0.5}}
	};
	const vertex3f normals[2] = {
		{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }
	};
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(x + 0.5f, y + 0.5f, z + 0.5f);
	glBindTexture(GL_TEXTURE_2D, TEXTURE[texture]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	for (int i = 0; i < 2; i++) {
		glBegin(GL_QUADS);
			glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
			glTexCoord2f(0.0f, 0.0f); glVertex3fv(cube[i][0]);
			glTexCoord2f(1.0f, 0.0f); glVertex3fv(cube[i][1]);
			glTexCoord2f(1.0f, 1.0f); glVertex3fv(cube[i][2]);
			glTexCoord2f(0.0f, 1.0f); glVertex3fv(cube[i][3]);
		glEnd();
	}
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

bool Plant::containPoint(float x, float y, float z)const {
	return false;
}

int Water::getThumbnailTexture()const {
	return texture;
}

/// <summary>
/// Water类构造函数
/// </summary>
/// <param name="texture_">材质类型</param>
Water::Water(int texture_) : texture(texture_), BlockBase(1, 1, 1, false) {}

/// <summary>
/// 在坐标(x, y, z)处渲染该方块。
/// </summary>
/// <param name="mask">渲染面掩码</param>
void Water::render(float x, float y, float z, unsigned char mask)const {
	if (!mask) return;
	typedef GLfloat vertex3[3];
	vertex3 cube[8] = {
		{ -0.5, -0.5, -0.5 }, { -0.5, 0.5, -0.5},
		{ -0.5, 0.5, 0.45 }, { -0.5, -0.5, 0.45},
		{ 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5},
		{ 0.5, 0.5, 0.45 }, { 0.5, -0.5, 0.45},
	};
	if (!((mask >> 4) & 1))
		for (int i = 2; i < 8; i += 4)
			cube[i][2] = cube[i + 1][2] = 0.5;
	const int vertexsOrder[6][4] = {
		{ 4, 5, 6, 7 }, { 1, 0, 3, 2 }, 
		{ 0, 4, 7, 3 }, { 5, 1, 2, 6 },
		{ 7, 6, 2, 3 }, { 4, 5, 1, 0 }
	};
	const GLfloat normals[6][3] = {
		{ 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }
	};
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(x + 0.5f, y + 0.5f, z + 0.5f);
	for (int i = 0; i < 6; i++) {
		if (!((mask >> i) & 1)) continue;

		glBindTexture(GL_TEXTURE_2D, TEXTURE[texture]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
			glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
			glTexCoord2f(0.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][0]]);
			glTexCoord2f(1.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][1]]);
			glTexCoord2f(1.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][2]]);
			glTexCoord2f(0.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][3]]);
		glEnd();
	}
	glPopMatrix();

}

bool Water::containPoint(float x, float y, float z)const {
	return false;
}

/// <summary>
/// 判断方块对type类型而言是否为填充类型，若是则内部面不会被渲染。
/// 水方块对于水方块而言是填充类型，对于其他类型而言不是填充类型。
/// </summary>
/// <param name="type'>方块类型</param>
/// <returns>方块若对type类型而言是填充类型则返回true，否则为false</returns>
bool Water::isFilledBlock(int type)const {
	if (type == WATER || type == LAVA)
		return true;
	else
		return false;
}
