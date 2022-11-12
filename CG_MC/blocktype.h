#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include <GL/GLUT.h>
#include "block.h"
#include "imagesplitter.h"
#include "imageloader.h"
#include "setting.h"

const size_t MAX_TEXTUER_NUM = 235;
extern GLuint TEXTURE[MAX_TEXTUER_NUM];

extern const block_t AIR;

extern const block_t BLOCK_TYPE_NUM;
extern const BlockBase* BLOCKS[];

void initTexture();
GLuint loadTexture(Image* image);

#endif
