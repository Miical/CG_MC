﻿#include <GL/glut.h>
#include <cstdio>
#include "display.h"
#include "map.h"
#include "setting.h"

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
    char s[10];
    sprintf_s(s, "FPS:%4.2f", FPS);
    glRasterPos2i(3, winHeight - 15);
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

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
}

GLfloat ag;
void displayFcn() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //设置光照
    glPushMatrix();
    ag += 0.2;
    glRotated(ag, 0, 1, 0);
    glTranslatef(2000, 0, 0);
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //RGBA模式下的镜面光
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glPopMatrix();

    // 物体
    glEnable(GL_TEXTURE_2D);
    worldMap.render();
    glDisable(GL_TEXTURE_2D);

    display2D();
    glutSwapBuffers();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    winWidth = newWidth, winHeight = newHeight;
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
        FPS = frameCount * 1000.0 / (currentTime - timeBase);
        printf("FPS:%4.2f\n", FPS);
        timeBase = currentTime;
        frameCount = 0;
    }

}