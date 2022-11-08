#ifndef SETTING_H
#define SETTING_H

#include <cstdio>
#include <GL/glut.h>

typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];
typedef unsigned char block_t;
extern const GLfloat eps;
extern const GLfloat PI;

extern GLint winWidth, winHeight;
extern const vertex3f INIT_POS;
extern const GLfloat ALPHA, GAMA;
extern const GLfloat VX, VY, VZ;

extern const GLfloat VISION;
extern const GLfloat XW_MIN, YW_MIN, XW_MAX, YW_MAX;
extern const GLfloat D_NEAR, D_FAR;

extern const GLfloat SPEED_Z;
extern const GLfloat WALK_SPEED;
extern const GLfloat MOUSE_SPEED;
extern const GLfloat FALLINT_SPEED;

extern const int MAP_BLOCK_SIZE;
extern const int WORLD_HEIGHT;
extern const int RENDER_RANGE;

#endif