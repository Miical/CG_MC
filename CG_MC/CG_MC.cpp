#include <GL/GLUT.h>
#include <math.h>
#include "imageloader.h"
#include "imagesplitter.h"
#include "blocktype.h"
#include "setting.h"
#include "inputprocess.h"
#include "character.h"
#include "map.h"

void drawFrontSight() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLineWidth(1.0);
    glBegin(GL_LINES);
		glVertex2f(winWidth / 2 - 10.0f, winHeight / 2);
		glVertex2f(winWidth / 2 + 10.0f, winHeight / 2);
		glVertex2f(winWidth / 2, winHeight / 2 - 10.0f);
		glVertex2f(winWidth / 2, winHeight / 2 + 10.0f);
    glEnd();
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
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

    drawFrontSight();
    glutSwapBuffers();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);

    winWidth = newWidth;
    winHeight = newHeight;
}

void idleFcn() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        character.getPosX(), 
        character.getPosY(), 
        character.getPosZ(),
		character.getRefPointX(), 
        character.getRefPointY(), 
        character.getRefPointZ(),
        VX, VY, VZ
    );

	inputIdleFunc();
    character.idleStateProcessing(worldMap);
    glutPostRedisplay();
}

void init(void) {
    glClearColor(0.6, 0.8, 0.9, 0.0);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(
        character.getPosX(), 
        character.getPosY(), 
        character.getPosZ(),
		character.getRefPointX(), 
        character.getRefPointY(), 
        character.getRefPointZ(),
        VX, VY, VZ
    );

    glMatrixMode(GL_PROJECTION);
    glFrustum(XW_MIN, XW_MAX, YW_MIN, YW_MAX, D_NEAR, D_FAR);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND); 
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    initTexture();
    initInput();
}

void exitFunc() {}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(150, 80);
    glutCreateWindow("CG_MC");

    /*
    glutGameModeString("1280x768");
    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
        glutEnterGameMode();
    else {
        printf("The select mode is not available\n");
        exit(1);
    }
    */

    atexit(exitFunc);

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutIdleFunc(idleFcn);
    glutMainLoop();
    return 0;
}