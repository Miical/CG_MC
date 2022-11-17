#include <cstdlib>
#include <windows.h>
#include "blocktype.h"
#include "map.h"
#include "mapgenerator.h"
#include "noise.h"

MapGenerator::MapGenerator(unsigned int seed_) : seed(seed_) {}

void MapGenerator::getChunk(block_t* blocks, BlocksSet& outBlocks, 
	int posX, int posY)const {
	terrainGeneration(blocks, posX, posY);
}

/// <summary>
/// 生成高度。
/// </summary>
/// <returns>高度值</returns>
int MapGenerator::heightGeneration(int x, int y)const {
	const float cystalSize[3] = { 4.0f, 32.0f, 96.0f };
	const float weight[3] = { 0.05f, 0.35f, 0.6f };
	const float maxHeight[3] = { WORLD_HEIGHT, WORLD_HEIGHT, WORLD_HEIGHT };
	float height = 0.0f;
	for (int i = 0; i < 3; i++)
		height += weight[i] * maxHeight[i] *
			perlin(Point2D(x / cystalSize[i], y / cystalSize[i]), seed) ;
	return int(height);
}

/// <summary>
/// 生成生物群落类型。
/// </summary>
Biome MapGenerator::biomeGeneration(int x, int y)const {
	const float cystalSize[2] = { 192.0f, 96.0f };
	float temperature = perlin(Point2D(x / cystalSize[0], y / cystalSize[0]), seed / 3);
	float humidity = perlin(Point2D(x / cystalSize[0], y / cystalSize[0]), seed / 2);
	if (temperature < 0.45) {
		if (humidity < 0.5)
			return Biome::Snow;
		else
			return Biome::ColdForest;
	}
	else if (temperature < 0.60) {
		if (humidity < 0.5)
			return Biome::Grassland;
		else
			return Biome::Forest;
	} else {
		if (humidity < 0.5)
			return Biome::Desert;
		else
			return Biome::RainForest;
	}
}


/// <summary>
/// 生成地形。
/// </summary>
void MapGenerator::terrainGeneration(
	block_t* blocks, int posX, int posY)const {
	for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			srand(0x7fffffff * 
				perlin(Point2D((posX + x) / 8.8f, (posY + y) / 8.8f), seed));
			int h = heightGeneration(posX + x, posY + y);
			Biome biome = biomeGeneration(posX + x, posY + y);
			for (int z = WORLD_HEIGHT - 1; z >= 0; z--)
				blocks[MapBlock::getID(x, y, z)] = AIR;

			// 铺设基岩
			int bedRockHeight = rand() % 3 + 2;
			for (int z = 0; z < bedRockHeight; z++)
				blocks[MapBlock::getID(x, y, z)] = BEDROCK;

			// 铺设近地土
			int dirtHeight = 4 + rand() % 5;
			for (int z = h - 1; z >= max(h - dirtHeight, bedRockHeight); z--) {
				if (biome == Biome::Desert && h >= SEA_LEVEL)
					blocks[MapBlock::getID(x, y, z)] = SAND;
				else
					blocks[MapBlock::getID(x, y, z)] = DIRT;
			}

			// 铺设中间矿石
			const int mineral[5] = {
				IRON_ORE, GOLD_ORE, DIAMOND_ORE,
				LAPIS_LAZULI_ORE, REDSTONE_ORE
			};
			for (int z = bedRockHeight; z < h - dirtHeight; z++) {
				if (rand() % 10 < 1)
					blocks[MapBlock::getID(x, y, z)] = mineral[rand() % 5];
				else
					blocks[MapBlock::getID(x, y, z)] = STONE;
			}

			// 铺设表层土
			if (h < SEA_LEVEL)
				blocks[MapBlock::getID(x, y, h)] = DIRT;
			else {
				switch (biome) {
				case Biome::Desert:
					blocks[MapBlock::getID(x, y, h)] = SAND;
					break;
				case Biome::ColdForest:
				case Biome::Snow:
					blocks[MapBlock::getID(x, y, h)] = SNOW;
					break;
				case Biome::Forest:
				case Biome::RainForest:
				case Biome::Grassland:
					blocks[MapBlock::getID(x, y, h)] = GRASS;
					break;
				default:
					blocks[MapBlock::getID(x, y, h)] = GRASS;
					break;
				}
			}
			
			// 铺设海平面
			if (h < SEA_LEVEL) {
				if (biome == Biome::ColdForest || biome == Biome::Snow)
					blocks[MapBlock::getID(x, y, SEA_LEVEL)] = ICE;
				else
					blocks[MapBlock::getID(x, y, SEA_LEVEL)] = WATER;
			}
			for (int z = max(SEA_LEVEL - 1, h); z > h; z--)
				blocks[MapBlock::getID(x, y, z)] = WATER;
			
			// 铺设空气
			for (int z = WORLD_HEIGHT - 1; z > max(h, SEA_LEVEL); z--)
				blocks[MapBlock::getID(x, y, z)] = AIR;
		}
	}


}
