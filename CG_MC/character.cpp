#include <cmath>
#include <windows.h>
#include "character.h"

Character character;

Character::Character() {
	for (int i = 0; i < 3; i++)
		pos[i] = INIT_POS[i];
	alpha = ALPHA; 
	gama = GAMA;
}

float Character::getPosX()const { return pos[0]; }
float Character::getPosY()const { return pos[1]; }
float Character::getPosZ()const { return pos[2]; }
float Character::getRefPointX()const { return pos[0] + sinf(gama) * cosf(alpha); }
float Character::getRefPointY()const { return pos[1] + sinf(gama) * sinf(alpha); }
float Character::getRefPointZ()const { return pos[2] + cosf(gama); }
float Character::getAlpha()const { return alpha; }
float Character::getGama()const { return gama; }


void Character::upOrDown(float dist) {
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
void Character::walk(float dist, int direct) {
	float angle = alpha + direct * PI / 2;
	pos[0] += dist * cosf(angle);
	pos[1] += dist * sinf(angle);
}
