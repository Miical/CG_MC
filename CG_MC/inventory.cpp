#include "inventory.h"
#include "blocktype.h"

Inventory inventory(BLOCKS, BLOCK_TYPE_NUM, winHeight, winWidth);

Inventory::Inventory(const BlockBase* blocks_[], int blocksNumber_,
		int windowsHeight_, int windowsWidth_):
	blocks(blocks_), blocksNumbers(blocksNumber_), 
	windowsHeight(windowsHeight_), windowsWidth(windowsWidth_),
	selectedBlock(0), blockListActive(false), selectedBlockInList(0) {
	for (int i = 1; i <= inventoryNum; i++)
		inventoryBlocks[i - 1] = i;
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
/// 重置大小，当窗口大小发生改变时调用。
/// </summary>
/// <return>当前方块类型</return>
void Inventory::reshapeSize(int windowsHeight_, int windowsWidth_) {
	windowsHeight = windowsHeight_;
	windowsWidth = windowsWidth_;
}


/// <summary>
/// 处理方块列表打开时鼠标点击事件。
/// </summary>
void Inventory::clickMouse(int x, int y) {
	y = winHeight - y;

	// 在底部物品栏中点击

	int gridSize = getGridSize();
	int posX = windowsWidth / 2 - inventoryNum * gridSize / 2,
		posY = gridSize * 0.3;
	if (posX <= x && x <= posX + gridSize * inventoryNum
		&& posY <= y && y <= posY + gridSize) {
		x -= posX;
		int id = x / gridSize;
		if (0 <= id && id < inventoryNum) {
			setSelectedBlock(id);
			setInventoryBlock(id, selectedBlockInList + 1);
		}
		return;
	}
	
	// 在物品列表中点击	

	y = windowsHeight - y;
	gridSize = gridSize * 1.2;
	posX = windowsWidth / 2 - blockListWidthNum * gridSize / 2;
	posY = gridSize * 0.3;
	if (posX <= x && x <= posX + gridSize * blockListWidthNum
		&& posY <= y && y <= posY + gridSize * blockListHeightNum) {
		y -= posY; x -= posX;
		int id = y / gridSize * blockListWidthNum + x / gridSize;
		if (0 <= id && id < BLOCK_TYPE_NUM)
			selectedBlockInList = id;
	}
}

/// <summary>
/// 查看物品列表是否打开。
/// </summary>
/// <return>若物品列表打开则返回true，否则为false</return>
bool Inventory::getBlockListStatus()const {
	return blockListActive;
}

/// <summary>
/// 打开物品列表。
/// </summary>
void Inventory::openBlockList() {
	blockListActive = true;
}

/// <summary>
/// 关闭物品列表。
/// </summary>
void Inventory::closeBlockList() {
	blockListActive = false;
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

/// <summary>
/// 渲染物品栏，若方块列表打开则渲染。
/// </summary>
void Inventory::render()const {
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	renderInventory();
	if (blockListActive)
		renderBlockList();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

/// <summary>
/// 设置物品栏中的物品。
/// 若与物品栏中其他格子拥有相同的类型，则将当前格子的类型移动至该格子。
/// </summary>
/// <param name="id">需要更改的物品栏格子序号</param>
/// <param name="blockType">需要设置的方块类型</param>
void Inventory::setInventoryBlock(int id, int blockType) {
	for (int i = 0; i < inventoryNum; i++) {
		if (i == id) continue;
		if (inventoryBlocks[i] == blockType) {
			inventoryBlocks[i] = inventoryBlocks[id];
			inventoryBlocks[id] = blockType;
		}
	}
	inventoryBlocks[id] = blockType;
}

/// <summary>
/// 渲染底部物品栏。
/// </summary>
void Inventory::renderInventory()const {
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

/// <summary>
/// 渲染方块列表。
/// </summary>
void Inventory::renderBlockList()const {
	int gridSize = getGridSize() * 1.2f;
	int posX, posY;
	GLfloat frameWidth = 3.5f;

	// 绘制边框

	posY = winHeight - gridSize * 0.3;
	for (int h = 0; h < blockListHeightNum; h++, posY -= gridSize) {
		posX = windowsWidth / 2 - blockListWidthNum * gridSize / 2;
		for (int i = 0; i < blockListWidthNum; i++, posX += gridSize) {
			glColor3f(0.35f, 0.35f, 0.35f);
			glBegin(GL_QUADS);
				glVertex2i(posX, posY);
				glVertex2i(posX + gridSize, posY);
				glVertex2i(posX + gridSize, posY - gridSize);
				glVertex2i(posX, posY - gridSize);
			glEnd();
		}
	}
	glColor3f(0.55f, 0.55f, 0.55f);
	posY = winHeight - gridSize * 0.3 
		- (selectedBlockInList / blockListWidthNum) * gridSize;
	posX = windowsWidth / 2 - blockListWidthNum * gridSize / 2 
		+ gridSize * (selectedBlockInList % blockListWidthNum);
	glBegin(GL_QUADS);
		glVertex2i(posX - frameWidth, posY + frameWidth);
		glVertex2i(posX + gridSize + frameWidth, posY + frameWidth);
		glVertex2i(posX + gridSize + frameWidth, posY - gridSize - frameWidth);
		glVertex2i(posX - frameWidth, posY - gridSize - frameWidth);
	glEnd();


	// 绘制黑色底色

	posY = winHeight - gridSize * 0.3;
	for (int h = 0; h < blockListHeightNum; h++, posY -= gridSize) {
		posX = windowsWidth / 2 - blockListWidthNum * gridSize / 2;
		for (int i = 0; i < blockListWidthNum; i++, posX += gridSize) {
			glColor3f(0.2f, 0.2f, 0.2f);
			glBegin(GL_QUADS);
				glVertex2i(posX + frameWidth, posY - frameWidth);
				glVertex2i(posX + gridSize - frameWidth, posY - frameWidth);
				glVertex2i(posX + gridSize - frameWidth, posY - gridSize + frameWidth);
				glVertex2i(posX + frameWidth, posY - gridSize + frameWidth);
			glEnd();
		}
	}

	// 绘制物品

	glColor3f(0.6f, 0.6f, 0.6f);
	glEnable(GL_TEXTURE_2D);
	posY = winHeight - gridSize * 0.3;
	frameWidth *= 3;
	for (int h = 0; h < blockListHeightNum; h++, posY -= gridSize) {
		posX = windowsWidth / 2 - blockListWidthNum * gridSize / 2;
		for (int i = 0; i < blockListWidthNum; i++, posX += gridSize) {
			int id = h * blockListWidthNum + i;
			if (id >= BLOCK_TYPE_NUM) break;
			glBindTexture(GL_TEXTURE_2D, TEXTURE[BLOCKS[id + 1]
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
	}
	glDisable(GL_TEXTURE_2D);
}

