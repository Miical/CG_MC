#include "blocktype.h"

const Image* image = loadBMP("./img/material.bmp3");
const ImageSplitter splitter(image, 16, 16);

GLuint TEXTURE[MAX_TEXTUER_NUM];

int texture_cnt;

/// <summary>
/// 材质导入辅助函数，以单张图片的形式导入特定行指定范围内的图片。
/// </summary>
void textureImportHelper(int row, int columnL, int columnR) {
    for (int i = columnL; i <= columnR; i++) {
        Image* img = splitter.getImage(row, i);
		TEXTURE[++texture_cnt] = loadTexture(img);
        delete img;
    }
}

/// <summary>
/// 材质导入辅助函数。导入一个两张图片所组成的材质
/// <param name="horizontal">图片是否水平排列</param>
/// </summary>
void textureImportHelper(int row, int coloum, bool horizontal) {
    if (horizontal) {
        Image* img = splitter.getImage(std::vector<int>{
			splitter.convertToID(row, coloum), 
			splitter.convertToID(row, coloum + 1) }, 
            true);
		TEXTURE[++texture_cnt] = loadTexture(img);
        delete img;
    }
    else {
        Image* img = splitter.getImage(std::vector<int>{
            splitter.convertToID(row, coloum),
			splitter.convertToID(row - 1, coloum) },
            false);
		TEXTURE[++texture_cnt] = loadTexture(img);
        delete img;
    }
}

void initTexture() {
    textureImportHelper(43, 0, 23);
    textureImportHelper(42, 0, 15);
    textureImportHelper(41, 0, 13);
    textureImportHelper(40, 0, 23);
    textureImportHelper(39, 0, 15);
    textureImportHelper(38, 0, 0);
    textureImportHelper(38, 1, false);
    textureImportHelper(38, 2, false);
    textureImportHelper(38, 3, 7);
    textureImportHelper(38, 19, 20);
    textureImportHelper(37, 0, 0);
    textureImportHelper(37, 3, 19);
    textureImportHelper(37, 20, true);
    textureImportHelper(37, 22, true);
    textureImportHelper(36, 0, 14);
    textureImportHelper(35, 0, 5);
    textureImportHelper(35, 6, true);
    textureImportHelper(35, 8, 9);
    textureImportHelper(34, 0, 5);
    textureImportHelper(34, 6, true);
    textureImportHelper(34, 8, 9);
    textureImportHelper(33, 0, 2);
    textureImportHelper(32, 0, 2);
    textureImportHelper(31, 0, 7);
    textureImportHelper(30, 0, 6);
    textureImportHelper(29, 0, 6);
    textureImportHelper(28, 0, 9);
    textureImportHelper(27, 0, 23);
    textureImportHelper(26, 0, 2);
    textureImportHelper(25, 0, 11);

    glMaterialfv(GL_FRONT, GL_AMBIENT, BLOCK_MAT_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, BLOCK_MAT_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, BLOCK_MAT_SPECULAR);
    glMaterialfv(GL_FRONT, GL_EMISSION, BLOCK_MAT_EMISSION);
    glMaterialf(GL_FRONT, GL_SHININESS, BLOCK_MAT_SHININESS);
}

const block_t BLOCK_TYPE_NUM = 72;
const BlockBase* BLOCKS[BLOCK_TYPE_NUM + 1];

/// <summary>
/// 导入方块。
/// </summary>
/// <return>导入后方块的编号</return>
int loadBoard(const BlockBase* block) {
    static int blockCnt = 0;
    BLOCKS[++blockCnt] = block;
    return blockCnt;
}

const block_t AIR = 0;
const block_t COBBLESTONE = loadBoard(new Block(25));
const block_t STONE_BRICKS = loadBoard(new Block(61));
const block_t MOSSY_STONE_BRICKS = loadBoard(new Block(107));
const block_t CRACKED_STONE_BRICKS = loadBoard(new Block(108));
const block_t CHISELED_STONE_BRICKS = loadBoard(new Block(177));
const block_t MOSS_STONE = loadBoard(new Block(45));
const block_t OAK_WOOD_PLANKS = loadBoard(new Block(5));
const block_t SPRUCE_WOOD_PLANKS = loadBoard(new Block(170));
const block_t BRICH_WOOD_PLANKS = loadBoard(new Block(178));
const block_t JUNGLE_WOOD_PLANKS = loadBoard(new Block(171));
const block_t ACACIA_WOOD_PLANKS = loadBoard(new Block(221));
const block_t DARK_OAK_WOOD_PLANKS = loadBoard(new Block(222));
const block_t BRICKS = loadBoard(new Block(8));
const block_t STONE = loadBoard(new Block(2));
const block_t GRANITE = loadBoard(new Block(231));
const block_t DIRT = loadBoard(new Block(3));
const int PODZOL_TEXTURE[6] = { 212, 212, 212, 212, 211, 3 };
const block_t PODZOL = loadBoard(new Block(PODZOL_TEXTURE));
const int GRASS_TEXTURE[6] = { 4, 4, 4, 4, 1, 3 };
const block_t GRASS = loadBoard(new Block(GRASS_TEXTURE));
const int SNOW_TEXTURE[6] = { 83, 83, 83, 83, 81, 3 };
const block_t SNOW = loadBoard(new Block(SNOW_TEXTURE));
const int MYCELIUM_TEXTURE[6] = { 92, 92, 92, 92, 93, 3 };
const block_t MYCELIUM = loadBoard(new Block(MYCELIUM_TEXTURE));
const int SANDSTONE_TEXTURE[6] = { 164, 164, 164, 164, 161, 172 };
const block_t SANDSTONE = loadBoard(new Block(SANDSTONE_TEXTURE));
const int CHISELED_SANDSTONE_TEXTURE[6] = { 184, 184, 184, 184, 161, 172 };
const block_t CHISELED_SANDSTONE = loadBoard(new Block(CHISELED_SANDSTONE_TEXTURE));
const int SMOOTH_SANDSTONE_TEXTURE[6] = { 185, 185, 185, 185, 161, 172 };
const block_t SMOOTH_SANDSTONE = loadBoard(new Block(SMOOTH_SANDSTONE_TEXTURE));
const block_t SAND = loadBoard(new Block(27));
const block_t RED_SAND = loadBoard(new Block(15));
const block_t GRAVEL = loadBoard(new Block(28));
const int OAK_WOOD_TEXTURE[6] = { 29, 29, 29, 29, 30, 30 };
const block_t OAK_WOOD = loadBoard(new Block(OAK_WOOD_TEXTURE));
const int SPRUCE_WOOD_TEXTURE[6] = { 129, 129, 129, 129, 214, 214 };
const block_t SPRUCE_WOOD = loadBoard(new Block(SPRUCE_WOOD_TEXTURE));
const int BRICH_WOOD_TEXTURE[6] = { 130, 130, 130, 130, 215, 215 };
const block_t BRICH_WOOD = loadBoard(new Block(BRICH_WOOD_TEXTURE));
const int JUNGLE_WOOD_TEXTURE[6] = { 157, 157, 157, 157, 216, 216 };
const block_t JUNGLE_WOOD = loadBoard(new Block(JUNGLE_WOOD_TEXTURE));
const int ACACIA_WOOD_TEXTURE[6] = { 217, 217, 217, 217, 218, 218 };
const block_t ACACIA_WOOD = loadBoard(new Block(ACACIA_WOOD_TEXTURE));
const int DARK_OAK_WOOD_TEXTURE[6] = { 219, 219, 219, 219, 220, 220 };
const block_t DARK_OAK_WOOD = loadBoard(new Block(DARK_OAK_WOOD_TEXTURE));
const block_t BEDROCK = loadBoard(new Block(26));
const block_t COAL_ORE = loadBoard(new Block(43));
const block_t IRON_ORE = loadBoard(new Block(42));
const block_t GOLD_ORE = loadBoard(new Block(41));
const block_t DIAMOND_ORE = loadBoard(new Block(57));
const block_t LAPIS_LAZULI_ORE = loadBoard(new Block(158));
const block_t REDSTONE_ORE = loadBoard(new Block(58));
const block_t OBSIDIAN = loadBoard(new Block(46));
const block_t ICE = loadBoard(new Block(82));
const block_t SNOW_BLOCK = loadBoard(new Block(81));
const block_t BIG_MUSHROOM_BOTTOM = loadBoard(new Block(139));
const block_t BIG_MUSHROOM_TOP = loadBoard(new Block(138));
const int MELON_TEXTURE[6] = { 147, 147, 147, 147, 148, 148 };
const block_t MELON = loadBoard(new Block(MELON_TEXTURE));
const int PUMPKIN_TEXTURE[6] = { 132, 131, 131, 131, 109, 109 };
const block_t PUMPKIN = loadBoard(new Block(PUMPKIN_TEXTURE));
const block_t CACTUS = loadBoard(new Plant(85));
const block_t WATER = loadBoard(new Water(233));
const block_t LAVA = loadBoard(new Water(234));
const block_t COBWEB = loadBoard(new Plant(12));
const block_t GLASS = loadBoard(new Leaves(56));
const block_t OAK_LEAVES = loadBoard(new Block(60));
const int SPRUCE_TEXTURE[6] = { 145, 145, 145, 145, 81, 145};
const block_t SPRUCE_LEAVES = loadBoard(new Block(SPRUCE_TEXTURE));
const block_t BIRCH_LEAVES = loadBoard(new Block(104));
const block_t JUNGLE_LEAVES = loadBoard(new Block(169));
const block_t GRASS_PLANT = loadBoard(new Plant(49));
const block_t DANDELION = loadBoard(new Plant(14));
const block_t POPPY = loadBoard(new Plant(13));
const block_t BLUT_ORCHID = loadBoard(new Plant(197));
const block_t ALLIUM = loadBoard(new Plant(198));
const block_t AZURE_BLUET = loadBoard(new Plant(199));
const block_t RED_TULIP = loadBoard(new Plant(200));
const block_t ORANGE_TULIP = loadBoard(new Plant(201));
const block_t WHITE_TULIP = loadBoard(new Plant(202));
const block_t PINK_TULIP = loadBoard(new Plant(203));
const block_t OXEYE_DAISY = loadBoard(new Plant(204));
const block_t SUNFLOWER = loadBoard(new Plant(205));
const block_t MUSHROOM = loadBoard(new Plant(38));
const block_t RED_MUSHROOM = loadBoard(new Plant(37));
const block_t YUAN_FLOWER = loadBoard(new Plant(71));
const block_t GHOST = loadBoard(new Plant(72));
const block_t TWO_MEOWS = loadBoard(new Plant(73));

/// <summary>
/// 导入材质。
/// </summary>
GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}

