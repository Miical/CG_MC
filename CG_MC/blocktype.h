#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include <GL/GLUT.h>
#include "block.h"
#include "imagesplitter.h"
#include "imageloader.h"
#include "setting.h"

const size_t MAX_TEXTUER_NUM = 100;
extern GLuint TEXTURE[MAX_TEXTUER_NUM];

extern const block_t AIR;
extern const block_t BLOCK_TYPE_NUM;

extern const BlockBase* BLOCKS[];
extern const Block DIRT;
extern const Block GRASS;
extern const Block STONE;
extern const Block OAK_WOOD_PLANK;
extern const Block COBBLESTONE;
extern const Leaves OAK_LEAVES;
extern const Plant GRASS_PLANT;

void initTexture();
GLuint loadTexture(Image* image);

#endif
