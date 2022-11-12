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

const int OAK_LEAVES_TEXTURE = 59;
const Leaves OAK_LEAVES(OAK_LEAVES_TEXTURE);
const int GRASS_PLANT_TEXTURE = 49;
const Plant GRASS_PLANT(GRASS_PLANT_TEXTURE);


const block_t AIR = 0;

const Block COBBLESTONE(25);
const Block STONE_BRICKS(61);
const Block MOSSY_STONE_BRICKS(107);
const Block CRACKED_STONE_BRICKS(108);
const Block CHISELED_STONE_BRICKS(177);
const Block MOSS_STONE(45);
const Block OAK_WOOD_PLANKS(5);
const Block SPRUCE_WOOD_PLANKS(170);
const Block BRICH_WOOD_PLANKS(178);
const Block JUNGLE_WOOD_PLANKS(171);
const Block ACACIA_WOOD_PLANKS(221);
const Block DARK_OAK_WOOD_PLANKS(222);
const Block BRICKS(8);
const Block STONE(2);
const Block GRANITE(231);
const Block DIRT(3);
const int PODZOL_TEXTURE[6] = { 212, 212, 212, 212, 211, 3 };
const Block PODZOL(PODZOL_TEXTURE);
const int GRASS_TEXTURE[6] = { 4, 4, 4, 4, 1, 3 };
const Block GRASS(GRASS_TEXTURE);
const int MYCELIUM_TEXTURE[6] = { 92, 92, 92, 92, 93, 3 };
const Block MYCELIUM(MYCELIUM_TEXTURE);
const int SANDSTONE_TEXTURE[6] = { 164, 164, 164, 164, 161, 172 };
const Block SANDSTONE(SANDSTONE_TEXTURE);
const int CHISELED_SANDSTONE_TEXTURE[6] = { 184, 184, 184, 184, 161, 172 };
const Block CHISELED_SANDSTONE(CHISELED_SANDSTONE_TEXTURE);
const int SMOOTH_SANDSTONE_TEXTURE[6] = { 185, 185, 185, 185, 161, 172 };
const Block SMOOTH_SANDSTONE(SMOOTH_SANDSTONE_TEXTURE);
const Block SAND(27);
const Block RED_SAND(15);
const Block GRAVEL(28);
const int OAK_WOOD_TEXTURE[6] = { 29, 29, 29, 29, 30, 30 };
const Block OAK_WOOD(OAK_WOOD_TEXTURE);

 
const block_t BLOCK_TYPE_NUM = 26;
const BlockBase* BLOCKS[BLOCK_TYPE_NUM + 1] = {
    nullptr, &COBBLESTONE, &STONE_BRICKS, &MOSSY_STONE_BRICKS, 
    &CRACKED_STONE_BRICKS, &CHISELED_STONE_BRICKS, &MOSS_STONE, 
    &OAK_WOOD_PLANKS, &SPRUCE_WOOD_PLANKS, &BRICH_WOOD_PLANKS, 
    &JUNGLE_WOOD_PLANKS, &ACACIA_WOOD_PLANKS, &DARK_OAK_WOOD_PLANKS, 
    &BRICKS, &STONE, &GRANITE, &DIRT, &PODZOL, &GRASS, &MYCELIUM, 
    &SANDSTONE, &CHISELED_SANDSTONE, &SMOOTH_SANDSTONE, &SAND, 
	&RED_SAND, &GRAVEL, &OAK_WOOD
};

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
        0,                            //0 for now
        GL_RGBA,                       //Format OpenGL uses for image
        image->width, image->height,  //Width and height
        0,                            //The border of the image
        GL_RGBA, //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
        //as unsigned numbers
        image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

