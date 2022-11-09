#include "block.h"
#include "blocktype.h"

BlockBase::BlockBase( int lengthNum_, int widthNum_, int heightNum_) :
	lengthNum(lengthNum_), heightNum(heightNum_), widthNum(widthNum_) {
}

void BlockBase::renderTargetBlock(float x, float y, float z)const {
	render(x, y, z);

	const vertex3f v[8] = {
		{ 0.0f, 0.0f, 0.0f }, { lengthNum, 0.0f, 0.0f },
		{ lengthNum, widthNum, 0.0f },{ 0.0f, widthNum, 0.0f }, 
		{ 0.0f, 0.0f, heightNum }, { lengthNum, 0.0f, heightNum },
		{ lengthNum, widthNum, heightNum }, { 0.0f, widthNum, heightNum }
	};

	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
}

Block::Block(const int texture_[6]) : BlockBase(1, 1, 1) {
	memcpy(texture, texture_, sizeof(texture));
}

void Block::render(float x, float y, float z)const {
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
