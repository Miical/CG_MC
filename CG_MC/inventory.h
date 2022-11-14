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
	void reshapeSize(int windowsHeight_, int windowsWidth_);
	void clickMouse(int x, int y);
	bool getBlockListStatus()const;
	void openBlockList();
	void closeBlockList();

	static const int inventoryNum = 10;
private:
	int inventoryBlocks[inventoryNum];
	int selectedBlock, selectedBlockInList;
	bool blockListActive;
	
	const BlockBase** blocks;
	const int blocksNumbers;
	
	static const int blockListWidthNum = 12;
	static const int blockListHeightNum = 6;
	static const int sizeRatio = 14;
	int windowsHeight, windowsWidth;

	void setInventoryBlock(int id, int blockType);
	void renderInventory()const;
	void renderBlockList()const;
	void loadFile();
	void saveFile()const;
};

extern Inventory inventory;

#endif
