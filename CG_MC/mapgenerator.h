#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "setting.h"

class MapGenerator {
public:
	void getChunk(block_t* blocks, int posX, int posY)const;
	MapGenerator(unsigned int seed_);

private:
	unsigned int seed;
	
	int heightGenerate(int x, int y)const;
};

extern const MapGenerator generator;
#endif 
