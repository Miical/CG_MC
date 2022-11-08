﻿#include <GL/glut.h>
#include <windows.h>
#include "inputprocess.h"
#include "character.h"
#include "setting.h"
#include "map.h"

bool mouseActive;

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		character.walk(WALK_SPEED, 0, worldMap);
		worldMap.changePos(character.getPosX(), character.getPosY());
		break;
	case 'a':
		character.walk(WALK_SPEED, 1, worldMap);
		worldMap.changePos(character.getPosX(), character.getPosY());
		break;
	case 's':
		character.walk(WALK_SPEED, 2, worldMap);
		worldMap.changePos(character.getPosX(), character.getPosY());
		break;
	case 'd':
		character.walk(WALK_SPEED, 3, worldMap);
		worldMap.changePos(character.getPosX(), character.getPosY());
		break;
	case 'm':
		character.reverseFly();
		break;
	case ' ':
		if (!character.isJumping())
			character.jump(worldMap);
		break;
	case 27:
		mouseActive = false;
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		break;
	case '~':
		exit(0);
		break;
	default:
		break;
	}
}

void mouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (!mouseActive) {
			glutWarpPointer(winWidth / 2, winHeight / 2);
			glutSetCursor(GLUT_CURSOR_NONE);
			mouseActive = true;
		}
	}
}

void passiveMotionFunc(int x, int y) {
	if (!mouseActive) return;
	
	GLfloat delta_x = GLfloat(x - winWidth / 2), delta_y = GLfloat(y - winHeight / 2);
	character.perspectiveChange(delta_x, delta_y);

	glutWarpPointer(winWidth / 2, winHeight / 2);
}

void specialKeyFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		if (character.isFlying()) {
			character.upOrDown(-SPEED_Z, worldMap);
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_UP:
		if (character.isFlying()) {
			character.upOrDown(SPEED_Z, worldMap);
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void initInput() {
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutSpecialFunc(specialKeyFunc);

	glutWarpPointer(winWidth / 2, winHeight / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	mouseActive = true;
}
