#include <GL/GLUT.h>
#include <math.h>
#include "blocktype.h"
#include "setting.h"
#include "inputprocess.h"
#include "character.h"
#include "map.h"
#include "display.h"

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
    displayIdleFunc();
    character.idleStateProcessing(worldMap);

    glutPostRedisplay();
}

void init(void) {
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
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    initTexture();
    initInput();
    initDisplay();
}

void exitFunc() {}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(150, 80);
    glutCreateWindow("CG_MC");
    atexit(exitFunc);

    /*
    glutGameModeString("1280x768");
    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
        glutEnterGameMode();
    else {
        printf("The select mode is not available\n");
        exit(1);
    }
    */

    init();
    glutIdleFunc(idleFcn);
    glutMainLoop();
    return 0;
}