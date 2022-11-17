#ifndef MAP_H
#define MAP_H
#include <map>
#include <set>
#include "setting.h"

typedef unsigned int MapBlockID;
typedef unsigned long long CompressedPos;
typedef std::map<CompressedPos, block_t> BlocksSet;
class MapBlock;
typedef std::map<MapBlockID, MapBlock*> MapBlocks;
struct DisplayNode {
	int x, y, z;
	unsigned char mask;
	block_t type;
	DisplayNode(int x_, int y_, int z_, unsigned char mask_, block_t type_) :
		x(x_), y(y_), z(z_), mask(mask_), type(type_) {}
	bool operator < (const DisplayNode& ots)const {
		if (x != ots.x) return x < ots.x;
		if (y != ots.y) return y < ots.y;
		return z < ots.z;
	}
};
typedef std::set<DisplayNode> DisplaySet;

class MapBlock {
public:
	MapBlock(int posX_, int posY_, 
		BlocksSet& modified_, MapBlocks& mapBlocks_);
	~MapBlock();

	bool contain(int x, int y, int z)const;
	block_t getBlock(int x, int y, int z)const;
	void modifyBlock(int x, int y, int z, block_t type);
	unsigned char getRenderMask(int x, int y, int z)const;
	DisplaySet& getDisplaySet();
	int getPosX()const { return posX; }
	int getPosY()const { return posY; }

	static int getID(int x, int y, int z);
	int getIDwithAbsolutePos(int x, int y, int z)const;

private:
	int posX, posY;
	block_t* blocks;
	BlocksSet outBlocks;
	DisplaySet display;
	BlocksSet& modified;
	MapBlocks& mapBlocks;

	bool isRender;
};


class Map {
public:
	Map(int watchPosX_, int watchPosY_, int watchPosZ_);
	~Map();
	block_t getBlock(int x, int y, int z);
	void setTargetBlock(Point3Di target);
	void setDropBlock(Point3Di drop);
	void changePos(int x, int y, int z);
	void render()const;
	void removeTargetBlock();
	void placeBlock(int type);

	void saveFile()const;
private:
	int watchPosX, watchPosY, watchPosZ;
	Point3Di targetBlock, dropBlock;
	std::map<MapBlockID, MapBlock*> mapBlocks;
	BlocksSet modified;

	bool inLoadRange(int x, int y)const;
	bool validMapBlock(MapBlock* mapBlock)const;
	
	const int saveInterval = 100;
	int saveCountdown;
	void loadFile();

	static unsigned char getRenderMask(block_t* blocks, int x, int y, int z);
};

CompressedPos compressPos(int x,int y, int z);
void decompressPos(CompressedPos pos, int& x, int& y, int& z);
MapBlockID getMapBlockID(int x, int y);
extern Map worldMap;

#endif 