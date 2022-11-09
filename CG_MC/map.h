#ifndef MAP_H
#define MAP_H
#include <map>
#include "setting.h"

typedef unsigned long long CompressedPos;
typedef std::map<CompressedPos, block_t> ModifiedBlocks;

class MapBlock {
public:
	MapBlock(int posX_, int posY_, ModifiedBlocks& modified_);
	~MapBlock();

	bool contain(int x, int y, int z)const;
	block_t getBlock(int x, int y, int z)const;
	void modifyBlock(int x, int y, int z, block_t type);
	int getPosX()const { return posX; }
	int getPosY()const { return posY; }

private:
	int getID(int x, int y, int z)const;

	block_t* blocks;
	int posX, posY;
	ModifiedBlocks& modified;
};


class Map {
public:
	Map(int watchPosX_, int watchPosY_);
	~Map();
	block_t getBlock(int x, int y, int z);
	void setTargetBlock(Point3Di target);
	void setDropBlock(Point3Di drop);
	void changePos(int x, int y);
	void render()const;
	void removeTargetBlock();

	void saveFile()const;
private:
	int watchPosX, watchPosY;
	Point3Di targetBlock, dropBlock;
	typedef unsigned int MapBlockID;
	std::map<MapBlockID, MapBlock*> mapBlocks;
	ModifiedBlocks modified;

	MapBlockID getMapBlockID(int x, int y)const;
	bool inRenderRange(int x, int y)const;
	bool validMapBlock(MapBlock* mapBlock)const;
	
	const int saveInterval = 100;
	int saveCountdown;
	void loadFile();
};

CompressedPos compressPos(int x, int y, int z);
extern Map worldMap;

#endif 