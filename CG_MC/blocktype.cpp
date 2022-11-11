#include "blocktype.h"

const Image* image = loadBMP("./img/material.bmp3");
const ImageSplitter splitter(image, 16, 16);

GLuint TEXTURE[MAX_TEXTUER_NUM];

void initTexture() {
    TEXTURE[0] = loadTexture(splitter.getImage(43, 0));
    TEXTURE[1] = loadTexture(splitter.getImage(43, 1));
	TEXTURE[2] = loadTexture(splitter.getImage(43, 2));
    TEXTURE[3] = loadTexture(splitter.getImage(43, 3));
    TEXTURE[4] = loadTexture(splitter.getImage(43, 4));
    TEXTURE[5] = loadTexture(splitter.getImage(43, 5));
    TEXTURE[6] = loadTexture(splitter.getImage(43, 6));
    TEXTURE[7] = loadTexture(splitter.getImage(43, 7));
    TEXTURE[8] = loadTexture(splitter.getImage(43, 8));
    TEXTURE[9] = loadTexture(splitter.getImage(43, 9));
    TEXTURE[10] = loadTexture(splitter.getImage(43, 10));
    TEXTURE[11] = loadTexture(splitter.getImage(42, 0));
    TEXTURE[12] = loadTexture(splitter.getImage(42, 1));
    TEXTURE[13] = loadTexture(splitter.getImage(42, 2));
    TEXTURE[14] = loadTexture(splitter.getImage(42, 3));
    TEXTURE[15] = loadTexture(splitter.getImage(42, 4));
    TEXTURE[16] = loadTexture(splitter.getImage(42, 5));
    TEXTURE[17] = loadTexture(splitter.getImage(42, 6));
    TEXTURE[18] = loadTexture(splitter.getImage(42, 7));
    TEXTURE[19] = loadTexture(splitter.getImage(42, 8));
    TEXTURE[20] = loadTexture(splitter.getImage(42, 9));
    TEXTURE[21] = loadTexture(splitter.getImage(42, 10));

    TEXTURE[22] = loadTexture(splitter.getImage(40, 4));
    TEXTURE[23] = loadTexture(splitter.getImage(41, 8));

    glMaterialfv(GL_FRONT, GL_AMBIENT, BLOCK_MAT_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, BLOCK_MAT_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, BLOCK_MAT_SPECULAR);
    glMaterialfv(GL_FRONT, GL_EMISSION, BLOCK_MAT_EMISSION);
    glMaterialf(GL_FRONT, GL_SHININESS, BLOCK_MAT_SHININESS);
}

const block_t AIR = 255;

const block_t BLOCK_TYPE_NUM = 7;
const BlockBase* BLOCKS[] = {
    &DIRT, &GRASS, &STONE, &OAK_WOOD_PLANK, &COBBLESTONE, &OAK_LEAVES, &GRASS_PLANT
};

const int DIRT_TEXTURE[6] = { 2, 2, 2, 2, 2, 2 };
const Block DIRT(DIRT_TEXTURE);
const int GRASS_TEXTURE[6] = { 3, 3, 3, 3, 0, 2 };
const Block GRASS(GRASS_TEXTURE);
const int STONE_TEXTURE[6] = { 1, 1, 1, 1, 1, 1 };
const Block STONE(STONE_TEXTURE);
const int OAK_WOOD_PLANK_TEXTURE[6] = { 4, 4, 4, 4, 4, 4 };
const Block OAK_WOOD_PLANK(OAK_WOOD_PLANK_TEXTURE);
const int COBBLESTONE_TEXTURE[6] = { 11, 11, 11, 11, 11, 11 };
const Block COBBLESTONE(COBBLESTONE_TEXTURE);
const int OAK_LEAVES_TEXTURE = 22;
const Leaves OAK_LEAVES(OAK_LEAVES_TEXTURE);
const int GRASS_PLANT_TEXTURE = 23;
const Plant GRASS_PLANT(GRASS_PLANT_TEXTURE);


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



