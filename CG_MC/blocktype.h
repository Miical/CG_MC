#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include <GL/GLUT.h>
#include "block.h"
#include "imagesplitter.h"
#include "imageloader.h"

const size_t MAX_TEXTUER_NUM = 100;
extern GLuint TEXTURE[MAX_TEXTUER_NUM];

extern const BlockBase* BLOCKS[];
extern const Block DIRT;
extern const Block GRASS;
extern const Block STONE;
extern const Block OAK_WOOD_PLANK;
extern const Block COBBLESTONE;

void initTexture();
GLuint loadTexture(Image* image);

#endif
