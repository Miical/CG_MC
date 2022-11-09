#include <cmath>
#include "vector3D.h"


const float eps = 1e-8;

bool fequal(float a, float b) {
    return fabs(a - b) < eps;
}

bool fzero(float x) {
    return fabs(x) < eps;
}

bool isZeroVector(Point3D v) {
    return fzero(v.x) && fzero(v.y) && fzero(v.x);
}

bool operator== (Point3D p1, Point3D p2) {
    return (fequal(p1.x, p2.x) && fequal(p1.y, p2.y) && fequal(p1.z, p2.z));
}

bool operator< (Point3D p1, Point3D p2) {
    if (!fequal(p1.x, p2.x))
        return (p1.x < p2.x);
    else if (!fequal(p1.y, p2.y))
        return (p1.y < p2.y);
    else
        return (p1.z < p2.z);
}

Point3D operator+ (Point3D p1, Point3D p2) {
    return Point3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Point3D operator-(Point3D p1, Point3D p2) {
    return Point3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Point3D operator* (Point3D a, float p) { 
    return Point3D(a.x * p, a.y * p, a.z * p); 
}

Point3D operator / (const Point3D& a, float p) { 
    return Point3D(a.x / p, a.y / p, a.z / p); 
}

// 计算叉乘 p1 x p2
Point3D operator* (Point3D p1, Point3D p2) {
    return Point3D(p1.y * p2.z - p1.z * p2.y,
        p1.z * p2.x - p1.x * p2.z,
        p1.x * p2.y - p1.y * p2.x);
}

// 计算点积 p1·p2
float operator&(Point3D p1, Point3D p2) { 
    return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

// 计算向量p的模
float Norm(Point3D p) {
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

//取平面法向量
Point3D GetV(Area3D s) {
    return (s.p1 - s.p2) * (s.p2 - s.p3);
}

//判三点共线
bool PointOnLine(Point3D p1, Point3D p2, Point3D p3) {
    return isZeroVector((p1 - p2) * (p2 - p3));
}

//判四点共面
int PointOnArea(Point3D a, Point3D b, Point3D c, Point3D d) {
    return fzero(GetV(Area3D(a, b, c)) & (d - a));
}

//求三维空间中两点间的距离
float Dis(Point3D p1, Point3D p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
// 求三维空间中点到直线的距离
float Dis(Point3D p, Line3D L) {
    if (L.p1 == L.p2) return Dis(p, L.p1);
    return Norm((p - L.p1) * (L.p2 - L.p1)) / Norm(L.p2 - L.p1);
}

// 判断三维空间中点p是否在直线L上
bool OnLine(Point3D p, Line3D L) {
    if (L.p1 == L.p2 && p == L.p1) 
        return true; //共点时，返回true
    return isZeroVector((p - L.p1) * (L.p2 - L.p1));
}

// 判断三维空间中点p是否在线段l上
bool OnLineSeg(Point3D p, Line3D L) {
    return (isZeroVector((L.p1 - p) * (L.p2 - p)) &&
        fequal(Norm(p - L.p1) + Norm(p - L.p2), Norm(L.p2 - L.p1)));
}

// 点p到平面Ap-Al的距离。
float Dis(Point3D p, Point3D Ap, Point3D Al) {
    // 如果不取绝对值，得到的是有向距离
    return fabs((p - Ap) & Al) / Norm(Al); 
}

// 点p在平面Ap-Al上的投影。
Point3D PointToArea(Point3D p, Point3D Ap, Point3D Al) {
    Al = Al / (Norm(Al));//把Al变成法向量。
    return p - Al * ((p - Ap) & Al);
}

//直线与平面交
int LineAndArea(Line3D l1, Point3D Ap, Point3D Al, Point3D & p)
{
	//输入直线，和平面的点法式
	//第一步，判断直线与平面是否平行。
	if (isZeroVector((l1.p2 - l1.p1) & Al)) 
        return 0;//直线与平面平行。
	double _t = ((Ap - l1.p1) & Al) / ((l1.p1 - l1.p2) & Al);
	p = l1.p1 + (l1.p1 - l1.p2) * _t;
	return 1;
}