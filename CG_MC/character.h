#ifndef CHARACTER_H
#define CHARACTER_H

#include "map.h"
#include "setting.h"

class Character {
public:
	Character();
	float getPosX()const;
	float getPosY()const;
	float getPosZ()const;
	float getRefPointX()const;
	float getRefPointY()const;
	float getRefPointZ()const;
	float getAlpha()const;
	float getGama()const;
	bool isFlying()const;
	bool isJumping()const;
	void reverseFly();

	void upOrDown(float dist, Map& world);
	void perspectiveChange(float deltaX, float deltaY);
	void walk(float dist, int direct, Map& world);
	void jump(Map& world);
	bool autoJump(Map& world);

	void idleStateProcessing(Map& world);
private:
	vertex3f pos;
	float alpha, gama;
	bool flying;
	int jumping;

	bool legalPos(float x, float y, float z, Map& world)const;
};

extern Character character;

#endif