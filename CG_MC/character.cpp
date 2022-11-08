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
	jumping = 0;
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
bool Character::isJumping()const { return jumping; }
void Character::reverseFly() { flying = !flying; }


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
	else if (!flying && !jumping)
		autoJump(world);
}

void Character::jump(Map& world) {
	if (!legalPos(pos[0], pos[1], pos[2] - 0.1f, world))
		jumping = 8;
}

bool Character::autoJump(Map& world) {
	float newPosX = pos[0] + WALK_SPEED * cosf(alpha),
		newPosY = pos[1] + WALK_SPEED * sinf(alpha),
		newPosZ = pos[2] + 1.05f;
	if (legalPos(newPosX, newPosY, newPosZ, world)) {
		jumping = 8;
		return true;
	}
	return false;
}

bool Character::legalPos(float x, float y, float z, Map& world)const {
	z -= 0.25f;
	for (float pz = z - 0.95f; pz <= z + 0.95f + eps; pz += 0.95f) {
		for (float px = x - 0.3f; px <= x + 0.3f + eps; px += 0.3f) {
			for (float py = y - 0.3f; py <= y + 0.3f + eps; py += 0.3f) {
				if (abs(px) <= eps || abs(py) <= eps) continue;
				if (z <= 0.0f || WORLD_HEIGHT <= z) return false;
				if (world.getBlock(floor(px), floor(py), floor(pz)) != AIR)
					return false;
			}
		}
	}
	return true;
}

void Character::idleStateProcessing(Map& world) {
	if (!flying) {
		if (!jumping && legalPos(pos[0], pos[1], pos[2] - FALLINT_SPEED, world))
			pos[2] -= FALLINT_SPEED;
		if (jumping) {
			const float interval = 0.15f;
			if (legalPos(pos[0], pos[1], pos[2] + interval, world)) {
				pos[2] += interval;
				jumping--;
			}
			else {
				jumping = 0;
			}

		}
	}
}
