#include <fstream>
#include <iostream>
#include "blocktype.h"
#include "map.h"
#include "mapgenerator.h"

Map worldMap(INIT_POS[0], INIT_POS[1], INIT_POS[2]);

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

Map::Map(int watchPosX_, int watchPosY_, int watchPosZ_) {
	loadFile();
	changePos(watchPosX_, watchPosY_, watchPosZ_);
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

void Map::changePos(int x, int y, int z) {
	watchPosX = x; watchPosY = y; watchPosZ = z;
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

/// <summary>
/// 渲染整张地图。
/// </summary>
void Map::render()const {

	// 遍历所有地图块，加载方块数据

	block_t* blocks = new block_t[RENDER_RANGE * RENDER_RANGE * WORLD_HEIGHT];
	int lx = watchPosX - RENDER_RANGE / 2, ly = watchPosY - RENDER_RANGE / 2;
	int rx = lx + RENDER_RANGE, ry = ly + RENDER_RANGE;
	for (auto& mapBlock : mapBlocks) {
		for (int i = 0, x = mapBlock.second->getPosX(); 
			i < MAP_BLOCK_SIZE; i++, x++) {
			
			for (int j = 0, y = mapBlock.second->getPosY(); 
				j < MAP_BLOCK_SIZE; j++, y++) {
				
				if (lx <= x && x < rx && ly <= y && y < ry) {
					for (int z = 0; z < WORLD_HEIGHT; z++) {
						block_t type = mapBlock.second->getBlock(x, y, z);
						blocks[(x - lx) * (RENDER_RANGE * WORLD_HEIGHT)
							+ (y - ly) * WORLD_HEIGHT + z] = type;
					}
				}
			}
		}
	}

	// 由远到近渲染方块，保证透明材质颜色信息正确性

	const int dx[] = { -1, 0, 1, 0 };
	const int dy[] = { 0, 1, 0, -1 };
	int currentDirect = 0, num = RENDER_RANGE * RENDER_RANGE;
	int x = rx - 1, y = ly;
	
	while (num--) {
		// 从低处和高处分别渲染至当前高度
		for (int z = 0; z <= watchPosZ; z++) {
			block_t& t = blocks[(x - lx) * (RENDER_RANGE * WORLD_HEIGHT)
				+ (y - ly) * WORLD_HEIGHT + z];
			if (t != AIR)
				if (x == targetBlock.x && y == targetBlock.y && z == targetBlock.z)
					BLOCKS[t]->renderTargetBlock(x, y, z);
				else BLOCKS[t]->render(x, y, z);
			t = INVALID_BLOCK;
		}
		for (int z = WORLD_HEIGHT - 1; z > watchPosZ; z--) {
			block_t& t = blocks[(x - lx) * (RENDER_RANGE * WORLD_HEIGHT)
				+ (y - ly) * WORLD_HEIGHT + z];
			if (t != AIR)
				if (x == targetBlock.x && y == targetBlock.y && z == targetBlock.z)
					BLOCKS[t]->renderTargetBlock(x, y, z);
				else BLOCKS[t]->render(x, y, z);
			t = INVALID_BLOCK;
		}

		// 获取下一个待渲染方块位置
		int nxtY = y + dy[currentDirect], nxtX = x + dx[currentDirect];
		if (lx <= nxtX && nxtX < rx && ly <= nxtY && nxtY < ry) {
			block_t& t = blocks[(nxtX - lx) * (RENDER_RANGE * WORLD_HEIGHT)
				+ (nxtY - ly) * WORLD_HEIGHT];
			if (t != INVALID_BLOCK) {
				y = nxtY; x = nxtX;
				continue;
			}
		} 
		currentDirect = (currentDirect + 1) % 4;
		y = y + dy[currentDirect], x = x + dx[currentDirect];
	}

	delete[] blocks;
}

void Map::removeTargetBlock() {
	if (targetBlock.z < 0) return;
	modified[compressPos(targetBlock.x, targetBlock.y, targetBlock.z)] = AIR;
	mapBlocks[getMapBlockID(targetBlock.x, targetBlock.y)]
		->modifyBlock(targetBlock.x, targetBlock.y, targetBlock.z, AIR);
}

void Map::placeBlock(int type) {
	if (dropBlock.z < 0) return;
	modified[compressPos(dropBlock.x, dropBlock.y, dropBlock.z)] = type;
	mapBlocks[getMapBlockID(dropBlock.x, dropBlock.y)]
		->modifyBlock(dropBlock.x, dropBlock.y, dropBlock.z, type);
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
	fstream fin;
	fin.open(MAP_FILE, fstream::in | fstream::app);
	if (!fin.is_open()) {
		cerr << "Load failed! Can't open file '" << MAP_FILE << "'.\n";
		cerr << "The world has been recreated." << endl;
		saveFile();
		return;
	}
	
	CompressedPos blockID;
	int type;
	while (fin >> blockID >> type)
		modified.insert(make_pair(blockID, type));
	fin.close();
}

CompressedPos compressPos(int x, int y, int z) {
	x += (1 << 27); y += (1 << 27);
	return ((CompressedPos)z << 56) + ((CompressedPos)y << 28) + ((CompressedPos)x);
}
