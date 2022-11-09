#ifndef VECTOR3D_H
#define VECTOR3D_H

struct Point3D { 
    float x, y, z;
    Point3D(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}
};

struct Line3D {
    Point3D p1, p2;
    Line3D(Point3D p1_, Point3D p2_) : p1(p1_), p2(p2_) {}
};

struct Area3D {
    Point3D p1, p2, p3;
    Area3D(Point3D p1_, Point3D p2_, Point3D p3_) : p1(p1_), p2(p2_), p3(p3_) {}
};

bool fequal(float a, float b);
bool fzero(float x);
bool isZeroVector(Point3D v);
bool operator== (Point3D p1, Point3D p2);
bool operator< (Point3D p1, Point3D p2);
Point3D operator+ (Point3D p1, Point3D p2);
Point3D operator- (Point3D p1, Point3D p2);
Point3D operator* (Point3D a, float p);
Point3D operator/ (const Point3D& a, float p);
Point3D operator* (Point3D p1, Point3D p2);
float operator&(Point3D p1, Point3D p2);
float Norm(Point3D p);
Point3D GetV(Area3D s);
bool PointOnLine(Point3D p1, Point3D p2, Point3D p3);
int PointOnArea(Point3D a, Point3D b, Point3D c, Point3D d);
float Dis(Point3D p1, Point3D p2);
float Dis(Point3D p, Line3D L);
bool OnLine(Point3D p, Line3D L);
bool OnLineSeg(Point3D p, Line3D L);
float Dis(Point3D p, Point3D Ap, Point3D Al);
int LineAndArea(Line3D l1, Point3D Ap, Point3D Al, Point3D & p);

#endif 
