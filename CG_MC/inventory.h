#ifndef INVENTORY_H
#define INVENTORY_H

#include "block.h"
#include "setting.h"

class Inventory {
public:
	Inventory(const BlockBase* blocks_[], int blocksNumber_,
		int windowsHeight_, int windowsWidth_);
	void render()const;
	int getGridSize()const;
	int getCurrentBlockType()const;
	int getSelectedBlock()const;
	void setSelectedBlock(int selectedBlock_);

	static const int inventoryNum = 10;
private:
	int inventoryBlocks[inventoryNum];
	int selectedBlock;
	
	const BlockBase** blocks;
	const int blocksNumbers;
	
	static const int sizeRatio = 14;
	int windowsHeight, windowsWidth;
};

extern Inventory inventory;

#endif
