#include <fstream>
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "blocktype.h"
#include "map.h"
#include "mapgenerator.h"

const char* MAP_FILE = "./data/map.dat";
int RENDER_RANGE = 10;
MapGenerator generator;
Map worldMap(INIT_POS[0], INIT_POS[1], INIT_POS[2]);

/// <summary>
/// 创建一个地图块。
/// </summary>
/// <param name="posX_">地图块在全局地图中x坐标</param>
/// <param name="posY_">地图块在全局地图中y坐标</param>
/// <param name="modified_">全局修改块集合</param>
/// <param name="mapBlocks_">全局中地图块的集合</param>
MapBlock::MapBlock(int posX_, int posY_,
	BlocksSet& modified_,
	MapBlocks& mapBlocks_ ): 
	posX(posX_), posY(posY_), 
	modified(modified_), mapBlocks(mapBlocks_),
	isRender(false) {

	int totalSize = WORLD_HEIGHT * MAP_BLOCK_SIZE * MAP_BLOCK_SIZE;
	blocks = new block_t[totalSize];
	
	// 从地形生成器获取初始地图
	generator.getChunk(blocks, &outBlocks, posX, posY);

	// outBlocks双向更新周边四个地图块
	const int dx[4] = { MAP_BLOCK_SIZE, -MAP_BLOCK_SIZE, 0, 0 };
	const int dy[4] = { 0, 0, MAP_BLOCK_SIZE, -MAP_BLOCK_SIZE };
	for (int i = 0; i < 4; i++) {
		MapBlockID id = getMapBlockID(posX + dx[i], posY + dy[i]);
		if (mapBlocks.find(id) == mapBlocks.end()) continue;
		MapBlock* otherBlock = mapBlocks[id];

		for (auto& block : outBlocks) {
			int x_, y_, z_;
			decompressPos(block.first, x_, y_, z_);
			if (otherBlock->contain(x_, y_, z_))
				otherBlock->blocks[ otherBlock->
					getIDwithAbsolutePos(x_, y_, z_)] = block.second;
		}

		for (auto& block : otherBlock->outBlocks) {
			int x_, y_, z_;
			decompressPos(block.first, x_, y_, z_);
			if (contain(x_, y_, z_))
				blocks[getIDwithAbsolutePos(x_, y_, z_)] = block.second;
		}
	}

	// 更新修改方块
	for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			for (int z = 0; z < WORLD_HEIGHT; z++) {
				if (modified.find(compressPos(x + posX, y + posY, z)) != modified.end())
					blocks[getID(x, y, z)] = modified[compressPos(x + posX, y + posY, z)];
			}
		}
	}
}

MapBlock::~MapBlock() {
	delete[] blocks;
}

/// <summary>
/// 该地图块中是否含有(x, y, z)点。
/// </summary>
bool MapBlock::contain(int x, int y, int z)const {
	return posX <= x && x < posX + MAP_BLOCK_SIZE
		&& posY <= y && y < posY + MAP_BLOCK_SIZE
		&& 0 <= z && z < WORLD_HEIGHT;
}


/// <summary>
/// 获取渲染掩码。
/// </summary>
/// <return>对应块的渲染面掩码</return>
unsigned char MapBlock::getRenderMask(int x, int y, int z)const {
	const int d[6][3] = {
		{ 1, 0, 0 }, { -1, 0, 0 },
		{ 0, -1, 0 }, { 0, 1, 0 },
		{ 0, 0, 1 }, { 0, 0, -1 }
	};
	unsigned char mask = 0u;
	for (int i = 0; i < 6; i++) {
		int px = x + d[i][0], py = y + d[i][1], pz = z + d[i][2];
		if (pz < 0 || WORLD_HEIGHT <= pz) continue;

		int type;
		if (0 <= px && px < MAP_BLOCK_SIZE 
			&& 0 <= py && py < MAP_BLOCK_SIZE) {
			type = blocks[getID(px, py, pz)];
		}
		else {
			MapBlock* b = mapBlocks[getMapBlockID(posX + px, posY + py)];
			type = b->blocks[b->getIDwithAbsolutePos(posX + px, posY + py, pz)];
		}

		if (type == AIR || !BLOCKS[type]->isFilledBlock(
			blocks[getID(x, y, z)]))
			mask |= (1u << i);
	}
	return mask;
}


/// <summary>
/// 获取显示集合。
/// </summary>
DisplaySet& MapBlock::getDisplaySet() {
	if (isRender) return display;
	isRender = true;

	for (int x = 0; x < MAP_BLOCK_SIZE; x++) {
		for (int y = 0; y < MAP_BLOCK_SIZE; y++) {
			for (int z = 0; z < WORLD_HEIGHT; z++) {
				if (blocks[getID(x, y, z)] == AIR)
					continue;
				unsigned char mask = getRenderMask(x, y, z);
				if (!mask) continue;
				display.insert(
					DisplayNode{ 
						posX + x, posY + y, z, 
						mask, blocks[getID(x, y, z)] 
					});
			}
		}
	}
	return display;
}

/// <summary>
/// 使用全局坐标获取方块类型
/// </summary>
block_t MapBlock::getBlock(int x, int y, int z)const {
	x -= getPosX(); y -= getPosY();
	return blocks[getID(x, y, z)];
}

/// <summary>
/// 修改(x, y, z)位置的方块
/// </summary>
void MapBlock::modifyBlock(int x, int y, int z, block_t type) {
	x -= getPosX(); y -= getPosY();
	blocks[getID(x, y, z)] = type;	
	
	// 更改自身以及周围六个方块的显示状态
	const int d[7][3] = {
		{ 0, 0, 0 },
		{ 1, 0, 0 }, { -1, 0, 0 },
		{ 0, -1, 0 }, { 0, 1, 0 },
		{ 0, 0, 1 }, { 0, 0, -1 }
	};	
	for (int i = 0; i < 7; i++) {
		int px = x + d[i][0], py = y + d[i][1], pz = z + d[i][2];
		if (pz < 0 || WORLD_HEIGHT <= pz) continue;

		if (0 <= px && px < MAP_BLOCK_SIZE 
			&& 0 <= py && py < MAP_BLOCK_SIZE) {
			auto it = display.find(DisplayNode(posX + px, posY + py, pz, 0, 0));
			if (it != display.end())
				display.erase(it);
			type = blocks[getID(px, py, pz)];

			if (type == AIR) continue;
			unsigned char mask = getRenderMask(px, py, pz);
			if (!mask) continue;
			display.insert(DisplayNode(posX + px, posY + py, pz, mask, type));
		}
		else {
			MapBlock* b = mapBlocks[getMapBlockID(posX + px, posY + py)];
			auto it = b->display.find(
				DisplayNode(posX + px, posY + py, pz, 0, 0));
			if (it != b->display.end())
				b->display.erase(it);
			type = b->blocks[b->getIDwithAbsolutePos(posX + px, posY + py, pz)];

			if (type == AIR) continue;
			unsigned char mask = b->getRenderMask(
				posX + px - b->getPosX(), posY + py - b->getPosY(), pz);
			if (!mask) continue;
			b->display.insert(DisplayNode(posX + px, posY + py, pz, mask, type));
		}
	}
}

/// <summary>
/// 相对坐标转换为方块数组下标。
/// </summary>
int MapBlock::getID(int x, int y, int z) {
	return x * (MAP_BLOCK_SIZE * WORLD_HEIGHT) + y * WORLD_HEIGHT + z;
}

/// <summary>
/// 全局坐标转换为方块数组下标。
/// </summary>
int MapBlock::getIDwithAbsolutePos(int x, int y, int z)const {
	return getID(x - posX, y - posY, z);
}

Map::Map(int watchPosX_, int watchPosY_, int watchPosZ_) {
	loadFile();
	changePos(watchPosX_, watchPosY_, watchPosZ_);
	targetBlock.x = 0; targetBlock.y = 0; targetBlock.z = -1;
}

Map::~Map() {
	saveFile();
}

/// <summary>
/// 获取(x, y, z)位置的方块类型
/// </summary>
block_t Map::getBlock(int x, int y, int z){
	return mapBlocks[getMapBlockID(x, y)]->getBlock(x, y, z);
}

/// <summary>
/// 设置目标方块位置
/// </summary>
void Map::setTargetBlock(Point3Di target) {
	targetBlock = target;
}

/// <summary>
/// 设置待放置方块位置
/// </summary>
void Map::setDropBlock(Point3Di drop) {
	dropBlock = drop;
}

/// <summary>
/// 改变观测位置至(x, y, z)。
/// </summary>
void Map::changePos(int x, int y, int z) {
	watchPosX = x; watchPosY = y; watchPosZ = z;
	for (auto mapBlock = mapBlocks.begin(); mapBlock != mapBlocks.end();) {
		if (!validMapBlock(mapBlock->second)) {
			delete mapBlock->second;
			mapBlock = mapBlocks.erase(mapBlock);
		}
		if (mapBlock != mapBlocks.end()) mapBlock++;
	}

	int lx = watchPosX - (RENDER_RANGE / 2 + 1) * MAP_BLOCK_SIZE,
		ly = watchPosY - (RENDER_RANGE / 2 + 1) * MAP_BLOCK_SIZE;
	int rx = lx + (RENDER_RANGE + 1) * MAP_BLOCK_SIZE,
		ry = ly + (RENDER_RANGE + 1) * MAP_BLOCK_SIZE;
	for (int x = lx; x <= rx; x += MAP_BLOCK_SIZE) {
		for (int y = ly; y <= ry; y += MAP_BLOCK_SIZE) {
			if (mapBlocks.find(getMapBlockID(x, y)) == mapBlocks.end()) {
				mapBlocks[getMapBlockID(x, y)] = new MapBlock(
					((x - (MAP_BLOCK_SIZE - 1) * (x < 0)) / MAP_BLOCK_SIZE) * MAP_BLOCK_SIZE,
					((y - (MAP_BLOCK_SIZE - 1) * (y < 0)) / MAP_BLOCK_SIZE) * MAP_BLOCK_SIZE,
					modified, mapBlocks);
			}
		} 
	}
}

/// <summary>
/// 渲染整张地图。
/// </summary>
void Map::render()const {

	// 保存所有需要显示的方块

	std::vector<DisplayNode> blocks;
	int lx = watchPosX - RENDER_RANGE / 2 * MAP_BLOCK_SIZE,
		ly = watchPosY - RENDER_RANGE / 2 * MAP_BLOCK_SIZE;
	int rx = lx + (RENDER_RANGE - 1) * MAP_BLOCK_SIZE,
		ry = ly + (RENDER_RANGE - 1) * MAP_BLOCK_SIZE;
	for (int x = lx; x <= rx; x += MAP_BLOCK_SIZE) {
		for (int y = ly; y <= ry; y += MAP_BLOCK_SIZE) {
			MapBlock* b = mapBlocks.find(getMapBlockID(x, y))->second;
			for (auto& node : b->getDisplaySet()) {
				blocks.push_back(node);
			}
		}
	}

	// 以正确顺序进行渲染，保证透明度渲染顺序需求

	std::sort(blocks.begin(), blocks.end(),
		[&](const DisplayNode& a, const DisplayNode& b) {
			if (abs(a.z - watchPosZ) != abs(b.z - watchPosZ))
				return abs(a.z - watchPosZ) > abs(b.z - watchPosZ);
			return max(abs(a.x - watchPosX), abs(a.y - watchPosY)) > 
				max(abs(b.x - watchPosX), abs(b.y - watchPosY));
		});
	for (auto& block : blocks) {
		if (block.x == targetBlock.x && block.y == targetBlock.y
			&& block.z == targetBlock.z)
			BLOCKS[block.type]->renderTargetBlock(
				block.x, block.y, block.z, block.mask);
		else
			BLOCKS[block.type]->render(block.x, block.y, block.z, block.mask);
	}
}

/// <summary>
/// 移除目标方块。
/// </summary>
void Map::removeTargetBlock() {
	if (targetBlock.z < 0) return;
	modified[compressPos(targetBlock.x, targetBlock.y, targetBlock.z)] = AIR;
	mapBlocks[getMapBlockID(targetBlock.x, targetBlock.y)]
		->modifyBlock(targetBlock.x, targetBlock.y, targetBlock.z, AIR);
}

/// <summary>
/// 在待放置位置放置类型为type的方块。
/// </summary>
void Map::placeBlock(int type) {
	if (dropBlock.z < 0) return;
	modified[compressPos(dropBlock.x, dropBlock.y, dropBlock.z)] = type;
	mapBlocks[getMapBlockID(dropBlock.x, dropBlock.y)]
		->modifyBlock(dropBlock.x, dropBlock.y, dropBlock.z, type);
}

/// <summary>
/// 根据坐标获取其所属的地图块ID.
/// </summary>
MapBlockID getMapBlockID(int x, int y) {
	unsigned int offset = 0x7fffffffu / 4u;
	return (unsigned int((y - (MAP_BLOCK_SIZE - 1) * (y < 0)) / MAP_BLOCK_SIZE + offset) << 16u)
		+ unsigned int((x - (MAP_BLOCK_SIZE - 1) * (x < 0)) / MAP_BLOCK_SIZE + offset);
}


/// <summary>
/// 位置(x, y)在加载范围内
/// </summary>
bool Map::inLoadRange(int x, int y)const {
	int lx = watchPosX - (RENDER_RANGE / 2 + 1) * MAP_BLOCK_SIZE,
		ly = watchPosY - (RENDER_RANGE / 2 + 1) * MAP_BLOCK_SIZE;
	int rx = lx + (RENDER_RANGE + 1) * MAP_BLOCK_SIZE,
		ry = ly + (RENDER_RANGE + 1) * MAP_BLOCK_SIZE;
	return lx <= x && x <= rx && ly <= y && y <= ry;
}

/// <summary>
/// 判断目标块是否还在加载范围内。
/// </summary>
bool Map::validMapBlock(MapBlock* mapBlock)const {
	int x = mapBlock->getPosX(), y = mapBlock->getPosY();
	return inLoadRange(x, y) 
		|| inLoadRange(x + MAP_BLOCK_SIZE - 1, y)
		|| inLoadRange(x, y + MAP_BLOCK_SIZE - 1) 
		|| inLoadRange(x + MAP_BLOCK_SIZE - 1, y + MAP_BLOCK_SIZE - 1);
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
	fin.open(MAP_FILE, fstream::in);
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

/// <summary>
///	压缩坐标 
/// </summary>
CompressedPos compressPos(int x, int y, int z) {
	x += (1 << 27); y += (1 << 27);
	return ((CompressedPos)z << 56) + ((CompressedPos)y << 28) + ((CompressedPos)x);
}

/// <summary>
///	解压坐标 
/// </summary>
void decompressPos(CompressedPos pos, int& x, int& y, int& z) {
	int x_ = pos % (1ull << 28), 
		y_ = pos / (1ull << 28) % (1ull << 28), 
		z_ = pos / (1ull << 56);
	x = x_ - (1 << 27);
	y = y_ - (1 << 27);
	z = z_;
}
