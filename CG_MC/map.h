#ifndef MAP_H
#define MAP_H
#include <map>
#include "setting.h"


class MapBlock {
public:
	MapBlock(int posX_, int posY_);
	~MapBlock();

	bool contain(int x, int y, int z)const;
	block_t getBlock(int x, int y, int z)const;
	int getPosX()const { return posX; }
	int getPosY()const { return posY; }

private:
	int getID(int x, int y, int z)const;

	block_t* blocks;
	int posX, posY;
};


class Map {
public:
	Map(int watchPosX_, int watchPosY_);
	block_t getBlock(int x, int y, int z);
	void changePos(int x, int y);

private:
	int watchPosX, watchPosY;
	typedef unsigned int MapBlockID;
	MapBlockID getMapBlockID(int x, int y)const;
	bool inRenderRange(int x, int y)const;
	bool validMapBlock(MapBlock* mapBlock)const;
	std::map<MapBlockID, MapBlock*> mapBlocks;
};

extern Map worldMap;

#endif 