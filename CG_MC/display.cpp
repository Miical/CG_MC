#include <GL/glut.h>
#include <cstdio>
#include "inputprocess.h"
#include "character.h"
#include "display.h"
#include "map.h"
#include "setting.h"
#include "inventory.h"

int frameCount, timeBase;
float FPS;

/// <summary>
/// 显示字符串。
/// </summary>
void drawString(const char* s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);
}

/// <summary>
/// 绘制准星。
/// </summary>
void drawFrontSight() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0);
    glBegin(GL_LINES);
		glVertex2f(winWidth / 2 - 10.0f, winHeight / 2);
		glVertex2f(winWidth / 2 + 10.0f, winHeight / 2);
		glVertex2f(winWidth / 2, winHeight / 2 - 10.0f);
		glVertex2f(winWidth / 2, winHeight / 2 + 10.0f);
    glEnd();
}

/// <summary>
/// 显示实时帧率。
/// </summary>
void displayFPS() {
    char s[11];
    sprintf_s(s, "FPS: %4.2f", FPS);
    glRasterPos2i(3, winHeight - 15);
    drawString(s);
}

/// <summary>
/// 显示坐标。
/// </summary>
void displayPos() {
    char s[30];
    sprintf_s(s, "POS: [%d %d %d]",
        int(floor(character.getPosX())),
        int(floor(character.getPosZ())),
        int(floor(character.getPosY())));
    glRasterPos2i(3, winHeight - 30);
    drawString(s);
}


/// <summary>
/// 显示2D内容。
/// </summary>
void display2D() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    drawFrontSight();
    displayFPS();
    displayPos();
    inventory.render();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
}

GLfloat ag;
void setLight() {
    ag += 0.001f;
    if (ag >= 2 * PI) ag = 0.0f;
    GLfloat light_position[] = { 
        character.getPosX() + 2000.0f * cosf(ag), 
        character.getPosY(),
        sinf(ag) * 2000.0f, 
        1.0f};
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    float rate = sin(ag);
	glClearColor(0.35 + 0.25 * rate, 
		0.45 + 0.35 * rate, 
		0.50 + 0.40 * rate, 0.0);
}

void displayFcn() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    setLight();

    glEnable(GL_TEXTURE_2D);
    worldMap.render();
    glDisable(GL_TEXTURE_2D);

    display2D();
    glutSwapBuffers();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    winWidth = newWidth, winHeight = newHeight;
    inventory.reshapeSize(winHeight, winWidth);
}

void initDisplay() {
    glClearColor(0.6, 0.8, 0.9, 0.0);
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);

    frameCount = 0; 
    timeBase = 0;
    FPS = 0.0;
}

void displayIdleFunc() {
    // 计算实时帧率
    frameCount++;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - timeBase > 1000) {
        if (currentTime > 3000)
            FPS = frameCount * 1000.0 / (currentTime - timeBase);
        else FPS = STANDARD_FPS;
        timeBase = currentTime;
        frameCount = 0;
    }
}