#ifndef INPUT_PROCESS
#define INPUT_PROCESS
#include "setting.h"

void initInput();
bool getTargetBlock(Point3Di& target);
bool getDropPos(Point3Di& target);
void inputIdleFunc();

extern float speedRatio;

#endif 
