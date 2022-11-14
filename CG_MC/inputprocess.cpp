#include <GL/glut.h>
#include <windows.h>
#include <vector>
#include "inputprocess.h"
#include "character.h"
#include "blocktype.h"
#include "display.h"
#include "map.h"
#include "inventory.h"
#include "vector3D.h"

bool mouseActive, leftButtonPressed, walkKeyPressed, upOrDownPressed;
int leftButtonTimer, currentDirect, currentUpDown;
float speedRatio;

/// <summary>
/// 释放鼠标。
/// </summary>
void releaseMouse() {
	mouseActive = false;
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

/// <summary>
/// 激活鼠标。
/// </summary>
void activeMouse() {
	glutWarpPointer(winWidth / 2, winHeight / 2);
	glutSetCursor(GLUT_CURSOR_NONE);
	mouseActive = true;
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		if (!inventory.getBlockListStatus()) {
			walkKeyPressed = true;
			currentDirect = 0;
		}
		break;
	case 'a':
		if (!inventory.getBlockListStatus()) {
			walkKeyPressed = true;
			currentDirect = 1;
		}
		break;
	case 's':
		if (!inventory.getBlockListStatus()) {
			walkKeyPressed = true;
			currentDirect = 2;
		}
		break;
	case 'd':
		if (!inventory.getBlockListStatus()) {
			walkKeyPressed = true;
			currentDirect = 3;
		}
		break;
	case 'm':
		if (!inventory.getBlockListStatus())
			character.reverseFly();
		break;
	case 'e':
		if (inventory.getBlockListStatus()) {
			inventory.closeBlockList();
			activeMouse();
		}
		else {
			inventory.openBlockList();
			releaseMouse();
		}
		break;
	case ' ':
		if (!inventory.getBlockListStatus()) {
			if (!character.isJumping())
				character.jump(worldMap);
		}
		break;
	case 19:
		worldMap.saveFile();
		break;
	case 27:
		releaseMouse();
		break;
	case '~':
		exit(0);
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '0':
		if ('0' <= key && key <= min('9', Inventory::inventoryNum + '0' - 1)) {
			if (key == '0')
				inventory.setSelectedBlock(9);
			else
				inventory.setSelectedBlock(key - '1');
		}
		break;
	default:
		break;
	}
}

void keyboardUpFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'a':
	case 's':
	case 'd':
		walkKeyPressed = false;
		break;
	default:
		break;
	}
}

void mouseFunc(int button, int state, int x, int y) {
	if (inventory.getBlockListStatus()) {
		if (state == GLUT_DOWN)
			inventory.clickMouse(x, y);
		return;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!mouseActive) {
			activeMouse();
		}
		else {
			leftButtonPressed = true;
			leftButtonTimer = 0;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (!leftButtonPressed) return;
		leftButtonPressed = false;
		
		worldMap.placeBlock(inventory.getCurrentBlockType());
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
	if (inventory.getBlockListStatus())
		return;

	switch (key) {
	case GLUT_KEY_DOWN:
		currentUpDown = 0;
		upOrDownPressed = true;
		break;
	case GLUT_KEY_UP:
		currentUpDown = 1;
		upOrDownPressed = true;
		break;
	case GLUT_KEY_LEFT:
		inventory.setSelectedBlock(
			(inventory.getSelectedBlock() + Inventory::inventoryNum - 1) 
			% Inventory::inventoryNum);
		break;
	case GLUT_KEY_RIGHT:
		inventory.setSelectedBlock(
			(inventory.getSelectedBlock() + 1) % Inventory::inventoryNum);
		break;
	default:
		break;
	}
}

void specialUpFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		upOrDownPressed = false;
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
	glutKeyboardUpFunc(keyboardUpFunc);
	glutSpecialFunc(specialKeyFunc);
	glutSpecialUpFunc(specialUpFunc);

	activeMouse();
	leftButtonTimer = 0;
	leftButtonPressed = false;
	upOrDownPressed = false;
	walkKeyPressed = false;
	currentDirect = 0;
	currentUpDown = 0;
	speedRatio = 1.0;
}

void inputIdleFunc() {
	// 鼠标左键长按拆除方块
	if (leftButtonPressed) {
		leftButtonTimer++;
		if (leftButtonTimer > 15) {
			worldMap.removeTargetBlock();
			leftButtonPressed = false;
		}
	}

	// 定位目标方块
    Point3Di target;
	bool hasTargetBlock = getTargetBlock(target);
	if (!hasTargetBlock) target.z = -1;
	worldMap.setTargetBlock(target);

	// 定位待放置方块位置
	if (!hasTargetBlock || !getDropPos(target)
		|| !inventory.getCurrentBlockType()
		|| !character.legalPosToPlaceBlock(
			target.x, target.y, target.z, inventory.getCurrentBlockType())
		|| worldMap.getBlock(target.x, target.y, target.z) != AIR)
		target.z = -1;
	worldMap.setDropBlock(target);

	// 人物行走
	if (walkKeyPressed) {
		character.walk(WALK_SPEED * speedRatio, currentDirect, worldMap);
		worldMap.changePos(
			character.getPosX(), character.getPosY(), character.getPosZ());
	}

	// 人物上升下降
	if (character.isFlying() && upOrDownPressed)
		character.upOrDown(
			(currentUpDown == 0 ? -1.0 : 1.0) * speedRatio * SPEED_Z, worldMap);

	// 计算当前帧率速率比率
	speedRatio = STANDARD_FPS / FPS;
}

bool getTargetBlock(Point3Di& target) {
	Point3Df now{ character.getPosX(), character.getPosY(), character.getPosZ() };
	Point3Df direct = { character.getRefPointX() - now.x,
		character.getRefPointY() - now.y,
		character.getRefPointZ() - now.z
	};

	for (int i = 0; i < 20; i++) {
		now.x += direct.x / 4.0f; now.y += direct.y / 4.0f; now.z += direct.z / 4.0f;
		if (now.z <= 0.0f) break;
		if (worldMap.getBlock(floor(now.x), floor(now.y), floor(now.z)) != AIR) {
			target.x = floor(now.x), target.y = floor(now.y), target.z = floor(now.z);
			return true;
		}
	}
	return false;
}

bool inBlock(Point3D p, Point3Di block) {
	return block.x - eps <= p.x && p.x <= block.x + 1.0 + eps
		&& block.y - eps <= p.y && p.y <= block.y + 1.0 + eps
		&& block.z - eps <= p.z && p.z <= block.z + 1.0 + eps;
}

bool getDropPos(Point3Di& target) {
	Point3D cube[8] = {
		{ 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0}, 
		{ 0.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0}, 
		{ 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0}, 
		{ 1.0, 1.0, 1.0 }, { 1.0, 0.0, 1.0}, 
	};
	const int vertexsOrder[6][4] = {
		{ 4, 5, 6, 7 }, { 1, 0, 3, 2 }, 
		{ 0, 4, 7, 3 }, { 5, 1, 2, 6 },
		{ 7, 6, 2, 3 }, { 4, 5, 1, 0 }
	};
	for (int i = 0; i < 8; i++)
		cube[i].x += target.x, cube[i].y += target.y, cube[i].z += target.z;

	Point3D watchPoint = Point3D(
		character.getPosX(), character.getPosY(), character.getPosZ());
	std::vector<std::pair<Point3D, int> > legalPoints;
	for (int i = 0; i < 6; i++) {
		Point3D p;
		int num = LineAndArea(
			Line3D(
				watchPoint,
				Point3D(character.getRefPointX(),
					character.getRefPointY(),
					character.getRefPointZ()
				)
			),
			cube[vertexsOrder[i][0]],
			GetV(
				Area3D(
					cube[vertexsOrder[i][0]],
					cube[vertexsOrder[i][1]],
					cube[vertexsOrder[i][2]]
				)
			),
			p
		);
		if (num && inBlock(p, target))
			legalPoints.push_back(std::make_pair(p, i));
	}

	if (legalPoints.empty()) 
		return false;

	float minDist = 1e18;
	int id = 0;
	for (auto& point : legalPoints) {
		float d = Norm(point.first - watchPoint);
		if (d < minDist) {
			minDist = d;
			id = point.second;
		}
	}

	const int faceChange[6][3] = {
		{ 1, 0, 0 }, { -1, 0, 0 },
		{ 0, -1, 0 }, { 0, 1, 0 },
		{ 0, 0, 1 }, { 0, 0, -1 }
	};
	
	target.x += faceChange[id][0];
	target.y += faceChange[id][1];
	target.z += faceChange[id][2];
	return true;
}
