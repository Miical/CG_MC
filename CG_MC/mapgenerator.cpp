#include "mapgenerator.h"

MapGenerator::MapGenerator(unsigned int seed_) : seed(seed_) {}

block_t MapGenerator::getBlock(int x, int y, int z)const {
	if (z == 0 || z == 1)
		return 1;
	else
		return 255;
}

const MapGenerator generator(0x3f3f3f3f);
