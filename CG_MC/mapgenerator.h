#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "setting.h"

class MapGenerator {
public:
	block_t getBlock(int x, int y, int z)const;
	MapGenerator(unsigned int seed_);

private:
	unsigned int seed;
};

extern const MapGenerator generator;
#endif 
