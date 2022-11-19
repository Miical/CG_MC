#ifndef SETTING_H
#define SETTING_H

#include <cstdio>
#include <GL/glut.h>

typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];
typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];
typedef struct Point3Df { GLfloat x, y, z; };
typedef struct Point3Di { GLint x, y, z; };

typedef unsigned char block_t;
const GLfloat eps = 1e-8;
const GLfloat PI = 3.14159265358979323846;

extern GLint winWidth, winHeight;
const vertex3f INIT_POS = { 0.0f, 0.0f, 95.0f };
const GLfloat ALPHA = 0.0f, GAMA = PI / 2.0f;
const GLfloat VX = 0.0f, VY = 0.0f, VZ = 1.0f;

const GLfloat VISION = 0.08f;
const GLfloat XW_MIN = -2.4f * VISION, YW_MIN = -1.4f * VISION, 
    XW_MAX = 2.4f * VISION, YW_MAX = 1.4f * VISION;
const GLfloat D_NEAR = 0.20f, D_FAR = 300.0f;

const GLfloat STANDARD_FPS = 60.0f;
const GLfloat SPEED_Z = 0.05f;
const GLfloat WALK_SPEED = 0.08f;
const GLfloat MOUSE_SPEED = 1.5f;
const GLfloat FALLING_SPEED = 0.12f;

const int WORLD_HEIGHT = 96;
const int SEA_LEVEL = WORLD_HEIGHT / 2 - 2;
const int MAP_BLOCK_SIZE = 6;
extern int RENDER_RANGE;

const GLfloat BLOCK_MAT_AMBIENT[] = { 0.9f, 0.9f, 0.9f, 0.9f };
const GLfloat BLOCK_MAT_DIFFUSE[] = { 0.9f, 0.9f, 0.9f, 0.9f };
const GLfloat BLOCK_MAT_SPECULAR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
const GLfloat BLOCK_MAT_EMISSION[] = { 0.3f, 0.3f, 0.3f, 0.3f };
const GLfloat BLOCK_MAT_SHININESS = 1.0f;

#endif