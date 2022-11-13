#include "inventory.h"
#include "blocktype.h"

Inventory inventory(BLOCKS, BLOCK_TYPE_NUM, winHeight, winWidth);

Inventory::Inventory(const BlockBase* blocks_[], int blocksNumber_,
		int windowsHeight_, int windowsWidth_):
	blocks(blocks_), blocksNumbers(blocksNumber_), 
	windowsHeight(windowsHeight_), windowsWidth(windowsWidth_),
	selectedBlock(0) {
	for (int i = 1; i <= inventoryNum; i++)
		inventoryBlocks[i - 1] = i;
	// memset(inventoryBlocks, 0, sizeof(inventoryBlocks));
}

/// <summary>
/// 计算单个格子的大小，由窗口高度决定。
/// </summary>
/// <return>单个格子大小</return>
int Inventory::getGridSize()const {
	return windowsHeight / sizeRatio;
}

void Inventory::setSelectedBlock(int selectedBlock_) {
	selectedBlock = selectedBlock_;
}

/// <summary>
/// 获取当前方块类型。
/// </summary>
/// <return>当前方块类型</return>
int Inventory::getCurrentBlockType()const {
	return inventoryBlocks[selectedBlock];
}

/// <summary>
/// 获取物品栏中选中格子的编号。
/// </summary>
/// <return>物品栏中选中格子的编号</return>
int Inventory::getSelectedBlock()const {
	return selectedBlock;
}

void Inventory::render()const {
	int gridSize = getGridSize();

	// 绘制边框

	GLfloat frameWidth = 3.5f;
	int posX = windowsWidth / 2 - inventoryNum * gridSize / 2,
		posY = gridSize * 1.3;
	for (int i = 0; i < inventoryNum; i++, posX += gridSize) {
		glColor3f(0.35f, 0.35f, 0.35f);
		glBegin(GL_QUADS);
			glVertex2i(posX, posY);
			glVertex2i(posX + gridSize, posY);
			glVertex2i(posX + gridSize, posY - gridSize);
			glVertex2i(posX, posY - gridSize);
		glEnd();
	}
	posX = windowsWidth / 2 - inventoryNum * gridSize / 2 
		+ gridSize * selectedBlock;
	glColor3f(0.4f, 0.4f, 0.4f);
	glBegin(GL_QUADS);
		glVertex2i(posX - frameWidth, posY + frameWidth);
		glVertex2i(posX + gridSize + frameWidth, posY + frameWidth);
		glVertex2i(posX + gridSize + frameWidth, posY - gridSize - frameWidth);
		glVertex2i(posX - frameWidth, posY - gridSize - frameWidth);
	glEnd();

	// 绘制黑色底色

	posX = windowsWidth / 2 - inventoryNum * gridSize / 2,
	posY = gridSize * 1.3;
	for (int i = 0; i < inventoryNum; i++, posX += gridSize) {
		glColor3f(0.2f, 0.2f, 0.2f);
		glBegin(GL_QUADS);
			glVertex2i(posX + frameWidth, posY - frameWidth);
			glVertex2i(posX + gridSize - frameWidth, posY - frameWidth);
			glVertex2i(posX + gridSize - frameWidth, posY - gridSize + frameWidth);
			glVertex2i(posX + frameWidth, posY - gridSize + frameWidth);
		glEnd();
	}

	// 绘制物品

	glColor3f(0.6f, 0.6f, 0.6f);
	glEnable(GL_TEXTURE_2D);
	posX = windowsWidth / 2 - inventoryNum * gridSize / 2;
	posY = gridSize * 1.3;
	frameWidth *= 3;
	for (int i = 0; i < inventoryNum; i++, posX += gridSize) {
		if (!inventoryBlocks[i]) continue;
		glBindTexture(GL_TEXTURE_2D, TEXTURE[BLOCKS[inventoryBlocks[i]]
			->getThumbnailTexture()]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2i(posX + frameWidth, posY - gridSize + frameWidth);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2i(posX + gridSize - frameWidth, posY - gridSize + frameWidth);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2i(posX + gridSize - frameWidth, posY - frameWidth);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2i(posX + frameWidth, posY - frameWidth);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

