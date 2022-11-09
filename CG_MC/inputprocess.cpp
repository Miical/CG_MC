﻿#include <GL/glut.h>
#include <windows.h>
#include "inputprocess.h"
#include "character.h"
#include "blocktype.h"
#include "map.h"

bool mouseActive, leftButtonPressed;
int leftButtonTimer;

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
	case 19:
		worldMap.saveFile();
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!mouseActive) {
			glutWarpPointer(winWidth / 2, winHeight / 2);
			glutSetCursor(GLUT_CURSOR_NONE);
			mouseActive = true;
		}
		else {
			leftButtonPressed = true;
			leftButtonTimer = 0;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (!leftButtonPressed) return;
		leftButtonPressed = false;

		// TODO
	}
}

void passiveMotionFunc(int x, int y) {
	if (!mouseActive) return;
	
	GLfloat delta_x = GLfloat(x - winWidth / 2), delta_y = GLfloat(y - winHeight / 2);
	character.perspectiveChange(delta_x, delta_y);

	glutWarpPointer(winWidth / 2, winHeight / 2);
}

void mouseMotionFunc(int x, int y) {
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
	glutMotionFunc(mouseMotionFunc);
	glutSpecialFunc(specialKeyFunc);

	glutWarpPointer(winWidth / 2, winHeight / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	leftButtonTimer = 0;
	mouseActive = true;
	leftButtonPressed = false;
}

void inputIdleFunc() {
	if (leftButtonPressed) {
		leftButtonTimer++;
		if (leftButtonTimer > 15) {
			worldMap.removeTargetBlock();
			leftButtonPressed = false;
		}
	}
}

bool getTargetBlock(Point3Di& target) {
	Point3Df now{ character.getPosX(), character.getPosY(), character.getPosZ() };
	Point3Df direct = { character.getRefPointX() - now.x,
		character.getRefPointY() - now.y,
		character.getRefPointZ() - now.z
	};

	for (int i = 0; i < 20; i++) {
		now.x += direct.x / 4.0f; now.y += direct.y / 4.0f; now.z += direct.z / 4.0f;
		if (worldMap.getBlock(floor(now.x), floor(now.y), floor(now.z)) != AIR) {
			target.x = floor(now.x), target.y = floor(now.y), target.z = floor(now.z);
			return true;
		}
	}
	return false;
}

