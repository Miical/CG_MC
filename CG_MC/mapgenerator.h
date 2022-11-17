#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "setting.h"

enum class Biome{
	Grassland, Snow, Desert, RainForest, Forest, ColdForest
};

class MapGenerator {
public:
	void getChunk(block_t* blocks, BlocksSet& outBlocks, 
		int posX, int posY)const;
	MapGenerator(unsigned int seed_);

private:
	const unsigned int seed;
	
	int heightGeneration(int x, int y)const;
	Biome biomeGeneration(int x, int y)const;
	void terrainGeneration(block_t* blocks, int posX, int posY)const;

};
#endif 
