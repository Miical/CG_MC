#include <GL/GLUT.h>
#include <math.h>
#include "imageloader.h"
#include "imagesplitter.h"
#include "blocktype.h"
#include "setting.h"
#include "inputprocess.h"
#include "character.h"
#include "map.h"

GLfloat ag;
void displayFcn() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //设置光照
    glPushMatrix();
    ag += 0.2;
    glRotated(ag, 0, 1, 0);
    glTranslatef(200, 0, 0);
    GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式
    GLfloat light0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光
    GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光
    GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式下的镜面光
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    GLfloat mat0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //定义材质的环境光颜色
    GLfloat mat0_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f }; //定义材质的漫反射光颜色
    GLfloat mat0_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f }; //定义材质的镜面反射光颜色
    GLfloat mat0_emission[] = { 1.0f, 0.9f, 0.1f, 1.0f }; //定义材质的辐射光颜色
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat0_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat0_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat0_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat0_emission);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    // 物体
    GLfloat object_mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的环境光颜色
    GLfloat object_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的漫反射光颜色
    GLfloat object_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的镜面反射光颜色
    GLfloat object_mat_emission[] = { 0.5f, 0.5f, 0.5f, 0.5f }; //定义材质的辐射光颜色
    GLfloat object_mat_shininess = 100.0f; //定义材质的光泽程度
    glMaterialfv(GL_FRONT, GL_AMBIENT, object_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, object_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, object_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, object_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, object_mat_shininess);

    glEnable(GL_TEXTURE_2D);
    
    worldMap.render();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);

    winWidth = newWidth;
    winHeight = newHeight;
}

void idleFcn(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        character.getPosX(), 
        character.getPosY(), 
        character.getPosZ(),
		character.getRefPointX(), 
        character.getRefPointY(), 
        character.getRefPointZ(),
        VX, VY, VZ
    );

    glutPostRedisplay();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(
        character.getPosX(), 
        character.getPosY(), 
        character.getPosZ(),
		character.getRefPointX(), 
        character.getRefPointY(), 
        character.getRefPointZ(),
        VX, VY, VZ
    );

    glMatrixMode(GL_PROJECTION);
    glFrustum(XW_MIN, XW_MAX, YW_MIN, YW_MAX, D_NEAR, D_FAR);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    initTexture();
    initInput();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(150, 80);
    glutCreateWindow("graphics");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutIdleFunc(idleFcn);
    glutMainLoop();

    return 0;
}