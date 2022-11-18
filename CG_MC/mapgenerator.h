#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "setting.h"

enum class Biome{
	Grassland, Snow, Desert, RainForest, Forest, ColdForest, SP
};

const int OAK[2] = { OAK_WOOD, OAK_LEAVES };
const int SPRUCE[2] = { SPRUCE_WOOD, SPRUCE_LEAVES };
const int BIRCH[2] = { OAK_WOOD, BIRCH_LEAVES };
const int JUNGLE[2] = { JUNGLE_WOOD, JUNGLE_LEAVES };
const int SP_TREE[2] = { JUNGLE_WOOD, TWO_MEOWS };

class MapGenerator {
public:
	void getChunk(block_t* blocks_, BlocksSet* outBlocks_, 
		int posX_, int posY_);
	MapGenerator(unsigned int seed_);

private:
	const unsigned int seed;
	
	void heightGeneration();
	void biomeGeneration();
	void terrainGeneration();
	void vegetationGeneration();
	void putBlock(int x, int y, int z, block_t type);
	void putTree(const int tree[2], int x, int y, int z);

	int posX, posY;
	block_t* blocks;
	BlocksSet* outBlocks;

	Biome biomes[MAP_BLOCK_SIZE][MAP_BLOCK_SIZE];
	float temperature[MAP_BLOCK_SIZE][MAP_BLOCK_SIZE];
	float humidity[MAP_BLOCK_SIZE][MAP_BLOCK_SIZE];
	int heights[MAP_BLOCK_SIZE][MAP_BLOCK_SIZE];
};
#endif 
