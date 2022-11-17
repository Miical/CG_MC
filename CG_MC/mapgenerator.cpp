#include <cstdlib>
#include <windows.h>
#include "blocktype.h"
#include "map.h"
#include "mapgenerator.h"
#include "noise.h"

MapGenerator::MapGenerator(unsigned int seed_) : seed(seed_) {}

void MapGenerator::getChunk(block_t* blocks, BlocksSet& outBlocks, 
	int posX, int posY)const {
	for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			float h = heightGenerate(posX + x, posY + y);
			for (int z = WORLD_HEIGHT - 1; z > h; z--)
				blocks[MapBlock::getID(x, y, z)] = AIR;
			for (int z = max(SEA_LEVEL, h); z > h; z--)
				blocks[MapBlock::getID(x, y, z)] = WATER;
			for (int z = h; z >= 0; z--)
				blocks[MapBlock::getID(x, y, z)] = GRASS;
		}
	}
}

/// <summary>
/// 生成高度。
/// </summary>
/// <returns>高度值</returns>
int MapGenerator::heightGenerate(int x, int y)const {
	float cystalSize[3] = { 4.0f, 24.0f, 96.0f };
	float weight[3] = { 0.05f, 0.30f, 0.65f };
	float maxHeight[3] = { WORLD_HEIGHT, WORLD_HEIGHT, WORLD_HEIGHT };
	float height = 0.0f;
	for (int i = 0; i < 3; i++)
		height += weight[i] * maxHeight[i] *
			perlin(Point2D(x / cystalSize[i], y / cystalSize[i]), seed) ;
	return int(height);
}


const MapGenerator generator(0x234F248A);

