#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include <GL/GLUT.h>
#include "block.h"
#include "imagesplitter.h"
#include "imageloader.h"
#include "setting.h"

const size_t MAX_TEXTUER_NUM = 235;
extern GLuint TEXTURE[MAX_TEXTUER_NUM];

extern const block_t BLOCK_TYPE_NUM;
extern const BlockBase* BLOCKS[];

extern const block_t AIR;
extern const block_t COBBLESTONE;
extern const block_t STONE_BRICKS;
extern const block_t MOSSY_STONE_BRICKS;
extern const block_t CRACKED_STONE_BRICKS;
extern const block_t CHISELED_STONE_BRICKS;
extern const block_t MOSS_STONE;
extern const block_t OAK_WOOD_PLANKS;
extern const block_t SPRUCE_WOOD_PLANKS;
extern const block_t BRICH_WOOD_PLANKS;
extern const block_t JUNGLE_WOOD_PLANKS;
extern const block_t ACACIA_WOOD_PLANKS;
extern const block_t DARK_OAK_WOOD_PLANKS;
extern const block_t BRICKS;
extern const block_t STONE;
extern const block_t GRANITE;
extern const block_t DIRT;
extern const block_t PODZOL;
extern const block_t GRASS;
extern const block_t SNOW;
extern const block_t MYCELIUM;
extern const block_t SANDSTONE;
extern const block_t CHISELED_SANDSTONE;
extern const block_t SMOOTH_SANDSTONE;
extern const block_t SAND;
extern const block_t RED_SAND;
extern const block_t GRAVEL;
extern const block_t OAK_WOOD;
extern const block_t SPRUCE_WOOD;
extern const block_t BRICH_WOOD;
extern const block_t JUNGLE_WOOD_WOOD;
extern const block_t ACACIA_WOOD;
extern const block_t DARK_OAK_WOOD;
extern const block_t BEDROCK;
extern const block_t COAL_ORE;
extern const block_t IRON_ORE;
extern const block_t GOLD_ORE;
extern const block_t DIAMOND_ORE;
extern const block_t LAPIS_LAZULI_ORE;
extern const block_t REDSTONE_ORE;
extern const block_t OBSIDIAN;
extern const block_t ICE;
extern const block_t SNOW_BLOCK;
extern const block_t BIG_MUSHROOM_BOTTOM;
extern const block_t BIG_MUSHROOM_TOP;
extern const block_t MELON;
extern const block_t PUMPKIN;
extern const block_t WATER;
extern const block_t LAVA;
extern const block_t COBWEB;
extern const block_t GLASS;
extern const block_t OAK_LEAVES;
extern const block_t SPRUCE_LEAVES;
extern const block_t BIRCH_LEAVES;
extern const block_t JUNGLE_LEAVES;
extern const block_t GRASS_PLANT;
extern const block_t DANDELION;
extern const block_t POPPY;
extern const block_t BLUT_ORCHID;
extern const block_t ALLIUM;
extern const block_t AZURE_BLUET;
extern const block_t RED_TULIP;
extern const block_t ORANGE_TULIP;
extern const block_t WHITE_TULIP;
extern const block_t PINK_TULIP;
extern const block_t OXEYE_DAISY;
extern const block_t SUNFLOWER;
extern const block_t MUSHROOM;
extern const block_t RED_MUSHROOM;
extern const block_t YUAN_FLOWER;

void initTexture();
GLuint loadTexture(Image* image);

#endif
