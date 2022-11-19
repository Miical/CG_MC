#include <cmath>
#include <windows.h>
#include "character.h"
#include "blocktype.h"

Character character;

Character::Character() {
	for (int i = 0; i < 3; i++)
		pos[i] = INIT_POS[i];
	alpha = ALPHA; 
	gama = GAMA;
	flying = false;
	jumping = 0.0;
}

float Character::getPosX()const { return pos[0]; }
float Character::getPosY()const { return pos[1]; }
float Character::getPosZ()const { return pos[2]; }
float Character::getRefPointX()const { return pos[0] + sinf(gama) * cosf(alpha); }
float Character::getRefPointY()const { return pos[1] + sinf(gama) * sinf(alpha); }
float Character::getRefPointZ()const { return pos[2] + cosf(gama); }
float Character::getAlpha()const { return alpha; }
float Character::getGama()const { return gama; }
bool Character::isFlying()const { return flying;  }
bool Character::isJumping()const { return jumping > 0.0; }
void Character::reverseFly() { flying = !flying; }

/// <summary>
/// 判断在位置(x, y, z)放置方块是否会与人物位置冲突。
/// </summary>
/// <param name="type">方块类型</param>
/// <returns>若无冲突返回true，否则返回false</returns>
bool Character::legalPosToPlaceBlock(int x, int y, int z, block_t type) {
	if (type <= 0 && BLOCK_TYPE_NUM < type) 
		return false;
	float tx = pos[0], ty = pos[1], tz = pos[2];
	tz -= 0.25f;
	for (float pz = tz - 0.95f; pz <= tz + 0.95f + eps; pz += 0.95f) {
		for (float px = tx - 0.3f; px <= tx + 0.3f + eps; px += 0.3f) {
			for (float py = ty - 0.3f; py <= ty + 0.3f + eps; py += 0.3f) {
				if (abs(px) <= eps || abs(py) <= eps) continue;
				if (int(floor(px)) == x 
					&& int(floor(py)) == y
					&& int(floor(pz)) == z
					&& BLOCKS[type]->containPoint(
						px - floor(px),
						py - floor(py),
						pz - floor(pz)
					))
					return false;
			}
		}
	}
	return true;
}

void Character::upOrDown(float dist, Map& world) {
	if (legalPos(pos[0], pos[1], pos[2] + dist, world))
		pos[2] += dist;
}

void Character::perspectiveChange(float deltaX, float deltaY) {
	alpha += MOUSE_SPEED * deltaX / 1000.0f;
	gama += MOUSE_SPEED * deltaY / 1000.0f;

	if (alpha < 0.0) alpha += 2 * PI;
	if (alpha > 2 * PI) alpha -= 2 * PI;
	gama = max(gama, 0.01 * PI);
	gama = min(gama, 0.99f * PI);
}

/**
* @param direct 0 - 前, 1 - 左， 2 - 后， 3 - 右
*/
void Character::walk(float dist, int direct, Map& world) {
	float angle = alpha + direct * PI / 2;
	float newPosX = pos[0] + dist * cosf(angle), 
		newPosY = pos[1] + dist * sinf(angle);
	if (legalPos(newPosX, newPosY, pos[2], world))
		pos[0] = newPosX, pos[1] = newPosY;
	else if (!flying && !isJumping())
		autoJump(world);
}

void Character::jump(Map& world, float speedRatio) {
	if (!legalPos(pos[0], pos[1], 
		pos[2] - FALLING_SPEED * speedRatio - 0.05, world))
		jumping = 13.0;
}

bool Character::autoJump(Map& world) {
	float newPosX = pos[0] + WALK_SPEED * cosf(alpha),
		newPosY = pos[1] + WALK_SPEED * sinf(alpha),
		newPosZ = pos[2] + 1.05f;
	if (legalPos(newPosX, newPosY, newPosZ, world)) {
		if (!legalPos(pos[0], pos[1], pos[2] - 0.4f, world)) {
			jumping = 8.0;
			return true;
		}
	}
	return false;
}

/// <summary>
/// 判断人物是否可以移动到位置(x, y, z)，用于人物碰撞检测。
/// </summary>
/// <param name="world">人物所在地图</param>
/// <returns>若任务可以移动到该位置，返回true，否则为false</returns>
bool Character::legalPos(float x, float y, float z, Map& world)const {
	// 将人物视作长方体，选取边缘上12个点作为碰撞检测参考。
	z -= 0.25f;
	for (float pz = z - 0.95f; pz <= z + 0.95f + eps; pz += 0.95f) {
		for (float px = x - 0.3f; px <= x + 0.3f + eps; px += 0.3f) {
			for (float py = y - 0.3f; py <= y + 0.3f + eps; py += 0.3f) {
				if (abs(px) <= eps || abs(py) <= eps) continue;
				if (pz <= 0.0f || WORLD_HEIGHT <= pz) return false;

				int type = world.getBlock(floor(px), floor(py), floor(pz));
				if (type != AIR && BLOCKS[type]->containPoint(
					px - floor(px), py - floor(py), pz - floor(pz)))
					return false;
			}
		}
	}
	return true;
}

void Character::idleStateProcessing(Map& world, float speedRatio) {
	if (!flying) {
		if (!isJumping() && legalPos(pos[0], pos[1],
			pos[2] - FALLING_SPEED * speedRatio, world))
			pos[2] -= FALLING_SPEED * speedRatio;
		if (jumping > 0.0) {
			const float interval = 0.15f * speedRatio;
			if (legalPos(pos[0], pos[1], pos[2] + interval, world)) {
				pos[2] += interval;
				jumping -= speedRatio;
			}
			else {
				jumping = -1.0;
			}

		}
	}
}
