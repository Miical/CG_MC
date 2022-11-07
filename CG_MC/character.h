#ifndef CHARACTER_H
#define CHARACTER_H

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

	void upOrDown(float dist);
	void perspectiveChange(float deltaX, float deltaY);
	void walk(float dist, int direct);
private:
	vertex3f pos;
	float alpha, gama;
};

extern Character character;

#endif