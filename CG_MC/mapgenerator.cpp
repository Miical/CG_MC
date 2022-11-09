#include <cstdlib>
#include "blocktype.h"
#include "mapgenerator.h"

MapGenerator::MapGenerator(unsigned int seed_) : seed(seed_) {}

block_t MapGenerator::getBlock(int x, int y, int z)const {
	if (z > 5) return 255;
	srand(x * y * z + x + y + z); 
	for (int i = 1; i <= 5; i++) rand();
	if (rand() % 10 < 5)
		return rand() % 5;
	else
		return AIR;
}

const MapGenerator generator(0x3f3f3f3f);