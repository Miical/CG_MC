﻿#include "block.h"
#include "blocktype.h"

/// <summary>
/// BlockBase类构造函数。
/// </summary>
/// <param name="lengthNum_">方块高度所占格子数</param>
/// <param name="widthNum_">方块宽度所占格子数</param>
/// <param name="heightNum_">方块长度所占格子数</param>
BlockBase::BlockBase( int lengthNum_, int widthNum_, int heightNum_) :
	lengthNum(lengthNum_), heightNum(heightNum_), widthNum(widthNum_) {
}

/// <summary>
/// 在(x, y, z)位置渲染被准星瞄准的方块。
/// </summary>
void BlockBase::renderTargetBlock(float x, float y, float z)const {
	render(x, y, z);

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
/// Block类构造函数。
/// </summary>
/// <param name="texture_">六个面的材质类型，顺序为前后左右上下</param>
Block::Block(const int texture_[6]) : BlockBase(1, 1, 1) {
	memcpy(texture, texture_, sizeof(texture));
}

/// <summary>
/// Block类构造函数。
/// </summary>
/// <param name="texture_">材质类型，应用于六个面</param>
Block::Block(const int texture_) : BlockBase(1, 1, 1) {
	for (int i = 0; i < 6; i++)
		texture[i] = texture_;
}

/// <summary>
/// 在坐标(x, y, z)处渲染该方块。
/// </summary>
void Block::render(float x, float y, float z)const {
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
	
/// <summary>
/// Leaves类构造函数
/// </summary>
/// <param name="texture_">材质类型</param>
Leaves::Leaves(int texture_) : Block(texture_) {}

void Leaves::render(float x, float y, float z)const {
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	Block::render(x, y, z);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

/// <summary>
/// Plant类构造函数
/// </summary>
/// <param name="texture_">材质类型</param>
Plant::Plant(int texture_) : texture(texture_), BlockBase(1, 1, 1) {}

void Plant::render(float x, float y, float z)const {
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
