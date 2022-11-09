#include <fstream>
#include <iostream>
#include "blocktype.h"
#include "map.h"
#include "mapgenerator.h"

Map worldMap(INIT_POS[0], INIT_POS[1]);

MapBlock::MapBlock(int posX_, int posY_, ModifiedBlocks& modified_): 
	posX(posX_), posY(posY_), modified(modified_) {

	int totalSize = WORLD_HEIGHT * MAP_BLOCK_SIZE * MAP_BLOCK_SIZE;
	blocks = new block_t[totalSize];
	for (int z = 0; z < WORLD_HEIGHT; z++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
				if (modified.find(compressPos(x + posX, y + posY, z)) != modified.end())
					blocks[getID(x, y, z)] = modified[compressPos(x + posX, y + posY, z)];
				else 
					blocks[getID(x, y, z)] = generator.getBlock(x + posX, y + posY, z);
			}
		}
	}
}

MapBlock::~MapBlock() {
	delete[] blocks;
}

bool MapBlock::contain(int x, int y, int z)const {
	return posX <= x && x < posX + MAP_BLOCK_SIZE
		&& posY <= y && y < posY + MAP_BLOCK_SIZE
		&& 0 <= z && z < WORLD_HEIGHT;
}

block_t MapBlock::getBlock(int x, int y, int z)const {
	x -= getPosX(); y -= getPosY();
	return blocks[getID(x, y, z)];
}

void MapBlock::modifyBlock(int x, int y, int z, block_t type) {
	x -= getPosX(); y -= getPosY();
	blocks[getID(x, y, z)] = type;
}

int MapBlock::getID(int x, int y, int z)const {
	return z * (MAP_BLOCK_SIZE * MAP_BLOCK_SIZE) + y * MAP_BLOCK_SIZE + x;
}

Map::Map(int watchPosX_, int watchPosY_) {
	changePos(watchPosX_, watchPosY_);
	loadFile();
	targetBlock.x = 0; targetBlock.y = 0; targetBlock.z = -1;
}

Map::~Map() {
	saveFile();
}

block_t Map::getBlock(int x, int y, int z){
	return mapBlocks[getMapBlockID(x, y)]->getBlock(x, y, z);
}

void Map::setTargetBlock(Point3Di target) {
	targetBlock = target;
}

void Map::setDropBlock(Point3Di drop) {
	dropBlock = drop;
}

void Map::changePos(int x, int y) {
	watchPosX = x; watchPosY = y;
	int lx = x - RENDER_RANGE / 2, ly  = y - RENDER_RANGE / 2;
	for (auto mapBlock = mapBlocks.begin(); mapBlock != mapBlocks.end();) {
		if (!validMapBlock(mapBlock->second)) {
			delete mapBlock->second;
			mapBlock = mapBlocks.erase(mapBlock);
		}
		if (mapBlock != mapBlocks.end()) mapBlock++;
	}
	for (int x = lx, flagx = false;;) {
		for (int y = ly, flagy = false;;) {
			if (mapBlocks.find(getMapBlockID(x, y)) == mapBlocks.end()) {
				mapBlocks[getMapBlockID(x, y)] = new MapBlock(
					((x - (MAP_BLOCK_SIZE - 1) * (x < 0)) / MAP_BLOCK_SIZE) * MAP_BLOCK_SIZE,
					((y - (MAP_BLOCK_SIZE - 1) * (y < 0)) / MAP_BLOCK_SIZE) * MAP_BLOCK_SIZE,
					modified);
			}
			y += MAP_BLOCK_SIZE;
			if (y >= ly + RENDER_RANGE) {
				if (!flagy) flagy = true, y = ly + RENDER_RANGE - 1;
				else break;
			}
		} 
		x += MAP_BLOCK_SIZE;
		if (x >= lx + RENDER_RANGE) {
			if (!flagx) flagx = true, x = lx + RENDER_RANGE - 1;
			else break;
		}
	}
}

void Map::render()const {
	int lx = watchPosX - RENDER_RANGE / 2, ly = watchPosY - RENDER_RANGE / 2;
	int rx = lx + RENDER_RANGE, ry = ly + RENDER_RANGE;
	for (auto& mapBlock : mapBlocks) {
		for (int i = 0, x = mapBlock.second->getPosX(); 
			i < MAP_BLOCK_SIZE; i++, x++) {

			for (int j = 0, y = mapBlock.second->getPosY(); 
				j < MAP_BLOCK_SIZE; j++, y++) {
				
				if (lx <= x && x < rx && ly <= y && y <= ry) {
					for (int z = 0; z < WORLD_HEIGHT; z++) {
						block_t type = mapBlock.second->getBlock(x, y, z);
						if (x == dropBlock.x && y == dropBlock.y && z == dropBlock.z)
							DIRT.renderTargetBlock(x, y, z);
						if (type != AIR)
							if (x == targetBlock.x && y == targetBlock.y && z == targetBlock.z)
								BLOCKS[type]->renderTargetBlock(x, y, z);
							else BLOCKS[type]->render(x, y, z);
					}
				}
			}
		}
	}
}

void Map::removeTargetBlock() {
	if (targetBlock.z < 0) return;
	modified[compressPos(targetBlock.x, targetBlock.y, targetBlock.z)] = AIR;
	mapBlocks[getMapBlockID(targetBlock.x, targetBlock.y)]
		->modifyBlock(targetBlock.x, targetBlock.y, targetBlock.z, AIR);
}

Map::MapBlockID Map::getMapBlockID(int x, int y)const {
	unsigned int offset = 0x7fffffffu / 4u;
	return (unsigned int((y - (MAP_BLOCK_SIZE - 1) * (y < 0)) / MAP_BLOCK_SIZE + offset) << 16u)
		+ unsigned int((x - (MAP_BLOCK_SIZE - 1) * (x < 0)) / MAP_BLOCK_SIZE + offset);
}

bool Map::inRenderRange(int x, int y)const {
	int lx = watchPosX - RENDER_RANGE / 2, ly = watchPosY - RENDER_RANGE / 2;
	return lx <= x && x < lx + RENDER_RANGE
		&& ly <= y && y < ly + RENDER_RANGE;
}

bool Map::validMapBlock(MapBlock* mapBlock)const {
	int x = mapBlock->getPosX(), y = mapBlock->getPosY();
	return inRenderRange(x, y) 
		|| inRenderRange(x + MAP_BLOCK_SIZE - 1, y)
		|| inRenderRange(x, y + MAP_BLOCK_SIZE - 1) 
		|| inRenderRange(x + MAP_BLOCK_SIZE - 1, y + MAP_BLOCK_SIZE - 1);
}

void Map::saveFile()const {
	using namespace std;
	ofstream fout;
	fout.open(MAP_FILE, ios_base::out | ios_base::trunc);
	if (!fout.is_open()) {
		cerr << "Save failed! Can't open file '" << MAP_FILE << "'.\n";
		return;
	}

	for (auto& mapBlock : modified)
		fout << mapBlock.first << ' ' << (int)mapBlock.second << endl;
	fout.close();
}

void Map::loadFile() {
	using namespace std;
	ifstream fin;
	fin.open(MAP_FILE, ios_base::in);
	if (!fin.is_open()) {
		cerr << "Load failed! Can't open file '" << MAP_FILE << "'.\n";
		cerr << "The world has been recreated." << endl;
		saveFile();
		return;
	}
	
	MapBlockID blockID;
	block_t type;
	while (fin >> blockID >> type)
		modified.insert(make_pair(blockID, type));
	fin.close();
}

CompressedPos compressPos(int x, int y, int z) {
	x += (1 << 27); y += (1 << 27);
	return ((CompressedPos)z << 56) + ((CompressedPos)y << 28) + ((CompressedPos)x);
}
