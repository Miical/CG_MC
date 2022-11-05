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
}

const BlockBase* BLOCKS[] = {
    &DIRT
};

const int DIRT_TEXTURE[6] = { 2, 2, 2, 2, 2, 2 };
const int DIRT_TEXTURE_DIRECT[6] = { 0, 0, 0, 0, 0, 0 };
const Block DIRT(DIRT_TEXTURE, DIRT_TEXTURE_DIRECT);

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
        0,                            //0 for now
        GL_RGB,                       //Format OpenGL uses for image
        image->width, image->height,  //Width and height
        0,                            //The border of the image
        GL_RGB, //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
        //as unsigned numbers
        image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}



