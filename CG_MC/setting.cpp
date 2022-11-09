#include <cmath>
#include "setting.h"

typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];
const GLfloat eps = 1e-8;
const GLfloat PI = 3.14159265358979323846;

GLint winWidth = 1200, winHeight = 700;

const vertex3f INIT_POS = { 0.0f, 0.0f, 15.0f };
const GLfloat ALPHA = 0.0f, GAMA = PI / 2.0f; // 视线与x轴、z轴所成夹角
const GLfloat VX = 0.0f, VY = 0.0f, VZ = 1.0f;

const GLfloat VISION = 0.08f;
const GLfloat XW_MIN = -2.4f * VISION, YW_MIN = -1.4f * VISION, 
    XW_MAX = 2.4f * VISION, YW_MAX = 1.4f * VISION;
const GLfloat D_NEAR = 0.20f, D_FAR = 100.0f;

const GLfloat SPEED_Z = 0.1f;
const GLfloat WALK_SPEED = 0.1f;
const GLfloat MOUSE_SPEED = 1.5f;
const GLfloat FALLINT_SPEED = 0.1f;

const int WORLD_HEIGHT = 32;
const int MAP_BLOCK_SIZE = 16;
const int RENDER_RANGE = 2 * MAP_BLOCK_SIZE; 

const GLfloat BLOCK_MAT_AMBIENT[] = { 0.9f, 0.9f, 0.9f, 0.9f }; // 环境光颜色 
const GLfloat BLOCK_MAT_DIFFUSE[] = { 0.9f, 0.9f, 0.9f, 0.9f }; // 漫反射光颜色
const GLfloat BLOCK_MAT_SPECULAR[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 镜面反射光颜色
const GLfloat BLOCK_MAT_EMISSION[] = { 0.3f, 0.3f, 0.3f, 0.3f }; // 辐射光颜色
const GLfloat BLOCK_MAT_SHININESS = 1.0f; //定义材质的光泽程度

const char* MAP_FILE = "./data/map.dat";
