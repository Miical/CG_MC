#include "block.h"
#include "blocktype.h"

BlockBase::BlockBase(
	int heightNum_, int widthNum_,
	const float interval_[6],
	const int texture_[6], const int textureDirect_[6]) :
	heightNum(heightNum_), widthNum(widthNum_) {

	memcpy(interval, interval_, sizeof(interval));
	memcpy(texture, texture_, sizeof(texture));
	memcpy(textureDirect, textureDirect_, sizeof(textureDirect_));
}
void BlockBase::render(float x, float y, float z, int direct, int rotate)const {
    glBindTexture(GL_TEXTURE_2D, TEXTURE[texture[0]]);

    glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}

const float BLOCK_INTERVAL[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
Block::Block(const int texture_[6], const int textureDirect_[6]) :
	BlockBase(0, 0, BLOCK_INTERVAL, texture_, textureDirect_) {}

