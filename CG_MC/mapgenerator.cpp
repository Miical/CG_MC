#include <cstdlib>
#include <windows.h>
#include "blocktype.h"
#include "map.h"
#include "mapgenerator.h"
#include "noise.h"

MapGenerator::MapGenerator(unsigned int seed_) : seed(seed_) {}

void MapGenerator::getChunk(block_t* blocks_, BlocksSet* outBlocks_, 
	int posX_, int posY_){
	posX = posX_; posY = posY_;
	blocks = blocks_;
	outBlocks = outBlocks_;
	srand(0x7fffffff * perlin(Point2D(posX / 8.8f, posY / 8.8f), seed));
	
	heightGeneration();
	biomeGeneration();
	terrainGeneration();
	vegetationGeneration();
}

/// <summary>
/// 生成高度。
/// </summary>
void MapGenerator::heightGeneration() {
	const float cystalSize[3] = { 4.0f, 32.0f, 96.0f };
	const float weight[3] = { 0.05f, 0.35f, 0.6f };
	const float maxHeight[3] = { WORLD_HEIGHT, WORLD_HEIGHT, WORLD_HEIGHT };

	for (int x = posX; x < posX + MAP_BLOCK_SIZE; x++) {
		for (int y = posY; y < posY + MAP_BLOCK_SIZE; y++) {
			float height = 0.0f;
			for (int i = 0; i < 3; i++)
				height += weight[i] * maxHeight[i] *
					perlin(Point2D(x / cystalSize[i], y / cystalSize[i]), seed) ;
			heights[x - posX][y - posY] = int(height);
		}
	}
}

/// <summary>
/// 生成生物群落类型。
/// </summary>
void MapGenerator::biomeGeneration() {
	const float cystalSize[2] = { 256.0f, 192.0f };

	for (int x = posX; x < posX + MAP_BLOCK_SIZE; x++) {
		for (int y = posY; y < posY + MAP_BLOCK_SIZE; y++) {
			float temperature_ = perlin(Point2D(x / cystalSize[0], y / cystalSize[0]), seed / 3);
			temperature_ += (rand() % 10 / 1000.0f - 0.005f);
			temperature_ = min(max(temperature_, 0.0f), 1.0f);
			float humidity_ = perlin(Point2D(x / cystalSize[0], y / cystalSize[0]), seed / 2);
			humidity_ += (rand() % 10 / 1000.0f - 0.005f);
			humidity_ = min(max(humidity_, 0.0f), 1.0f);
			temperature[x - posX][y - posY] = temperature_;
			humidity[x - posX][y - posY] = humidity_;

			if (temperature_ < 0.45) {
				if (temperature_ < 0.4 && humidity_ >= 0.5) 
					biomes[x - posX][y - posY] = Biome::ColdForest;
				else 
					biomes[x - posX][y - posY] = Biome::Snow;
			}
			else if (temperature_ < 0.55) {
				if (0.50 <= temperature_ && humidity_ > 0.5) 
					biomes[x - posX][y - posY] = Biome::Forest;
				else 
					biomes[x - posX][y - posY] = Biome::Grassland;
			}
			else {
				if (humidity_ < 0.35)
					biomes[x - posX][y - posY] = Biome::SP;
				else if (humidity_ < 0.47)
					biomes[x - posX][y - posY] = Biome::Desert;
				else if (humidity_ < 0.5)
					biomes[x - posX][y - posY] = Biome::Grassland;
				else
					biomes[x - posX][y - posY] = Biome::RainForest;
			}
		}
	}
}

/// <summary>
/// 生成地形。
/// </summary>
void MapGenerator::terrainGeneration() {
	for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			int h = heights[x][y];
			Biome biome = biomes[x][y];
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
					if (rand() % 100 < 1)
						blocks[MapBlock::getID(x, y, h)] = DIRT;
					else 
						blocks[MapBlock::getID(x, y, h)] = SNOW;
					break;
				case Biome::Forest:
				case Biome::RainForest:
				case Biome::Grassland:
					if (rand() % 100 < 1)
						blocks[MapBlock::getID(x, y, h)] = DIRT;
					else 
						blocks[MapBlock::getID(x, y, h)] = GRASS;
					break;
				case Biome::SP:
					blocks[MapBlock::getID(x, y, h)] = PUMPKIN;
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

/// <summary>
/// 植被生成。
/// </summary>
void MapGenerator::vegetationGeneration() {
	static const int OAK[2] = { OAK_WOOD, OAK_LEAVES };
	static const int SPRUCE[2] = { SPRUCE_WOOD, SPRUCE_LEAVES };
	static const int BIRCH[2] = { OAK_WOOD, BIRCH_LEAVES };
	static const int JUNGLE[2] = { JUNGLE_WOOD, JUNGLE_LEAVES };
	static const int SP_TREE[2] = { JUNGLE_WOOD, TWO_MEOWS };

	float c[4] = { 0.1, 0.5, 0.35, 0.05 };
	for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			float p = c[0] * perlin(Point2D((x + posX) / 32.0,
				(y + posY) / 32.0), seed ^ 0x56AF2B91)
				+ c[1] * humidity[x][y]
				+ c[2] * temperature[x][y]
				+ c[3] * (rand() % 100 / 100.0f);
			if (rand() % 1000 / 1000.0f > (1.0 - p) / 2.0) continue;
			int h = heights[x][y] + 1;
			if (h - 3 <= SEA_LEVEL) continue;
			if (h >= WORLD_HEIGHT) continue;

			switch (biomes[x][y]) {
			case Biome::Snow:
				break;
			case Biome::ColdForest:
				if (rand() % 7 < 1)
					putTree(SPRUCE, x, y, h);
				break;
			case Biome::Grassland:
				blocks[MapBlock::getID(x, y, h)] = GRASS_PLANT;
				break;
			case Biome::Forest: {
				if (rand() % 15 < 1)
					putTree(OAK, x, y, h);
				else {
					const int num = 3;
					const int PLANT_FOREST[num] = { GRASS_PLANT, DANDELION, POPPY };
					const float P[num] = { 0.96, 0.02, 0.02 };
					for (int i = 0, t = rand() % 100 + 1; i < num; i++) {
						t -= P[i] * 100;
						if (t <= 0) {
							blocks[MapBlock::getID(x, y, h)] = PLANT_FOREST[i];
							break;
						}
					}
				}
				break;
			}
			case Biome::SP: {
				int r = rand() % 30;
				if (r < 1)
					putTree(SP_TREE, x, y, h);
				else if (r < 5)
					blocks[MapBlock::getID(x, y, h)] = GHOST;
				break;
			}
			case Biome::Desert:
				if (rand() % 20 < 1)
					blocks[MapBlock::getID(x, y, h)] = CACTUS;
				break;
			case Biome::RainForest: {
				if (rand() % 6 < 1)
					putTree(JUNGLE, x, y, h);
				else {
					const int num = 13;
					const int PLANT[num] = {
						GRASS_PLANT, DANDELION, POPPY,
						BLUT_ORCHID, ALLIUM, AZURE_BLUET,
						RED_TULIP, ORANGE_TULIP, WHITE_TULIP,
						PINK_TULIP, OXEYE_DAISY, SUNFLOWER, YUAN_FLOWER
					};

					const float P[num] = { 0.76, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 
						0.02, 0.02, 0.02, 0.02, 0.02, 0.02 };
					for (int i = 0, t = rand() % 100 + 1; i < num; i++) {
						t -= P[i] * 100;
						if (t <= 0) {
							blocks[MapBlock::getID(x, y, h)] = PLANT[i];
							break;
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

/// <summary>
/// 在(x, y, z)位置放置指定方块，如果超出该chunk，则将其放置在outBlocks中。
/// </summary>
void MapGenerator::putBlock(int x, int y, int z, block_t type) {
	if (z < 0 || z >= WORLD_HEIGHT) return;
	if (0 <= x && x < MAP_BLOCK_SIZE
		&& 0 <= y && y < MAP_BLOCK_SIZE) {
		blocks[MapBlock::getID(x, y, z)] = type;
	}
	else {
		(* outBlocks)[compressPos(x + posX, y + posY, z)] = type;
	}
}

/// <summary>
/// 在(x, y, z)位置放置一棵指定类型的树。
/// </summary>
void MapGenerator::putTree(const int tree[2], int x, int y, int z) {
	int treeHeight = 8 + rand() % 4;
	int leavesHeight = treeHeight * 2 / 3;
	int trunkHeight = treeHeight * 3 / 5;
	
	Point2D p[4] = {
		Point2D(0, 0),
		Point2D(treeHeight / 3, treeHeight * 2 / 5 + 
			rand() % (treeHeight / 6)),
		Point2D(treeHeight * 2 / 3, treeHeight / 5 +
			rand() % (treeHeight / 4)),
		Point2D(treeHeight, 0)
	};
	
	for (int i = 0; i <= leavesHeight; i++) {
		float t = 1.0f * i / leavesHeight;
		float b = (p[0] * (1 - t * t * t) + p[1] * 3.0 * t * (1 - t) * (1 - t)
			+ p[2] * 3.0 * t * t * (1 - t) + p[3] * t * t * t).y;
		int r = round(b);
		
		for (int px = x - r; px <= x + r; px++) {
			for (int py = y - r; py <= y + r; py++) {
				if ((px == x - r || px == x + r) && (py == y - r || py == y + r))
					continue;
				putBlock(px, py, z + i + treeHeight - leavesHeight - 2, 
					tree[1]);
			}
		}
	}

	for (int i = 0; i < trunkHeight; i++)
		putBlock(x, y, z + i, tree[0]);
}
