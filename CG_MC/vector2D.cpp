#include <cmath>
#include "vector2D.h"

Vector2D operator +(Vector2D A, Vector2D B) { return Vector2D(A.x + B.x, A.y + B.y); }
Vector2D operator -(Point2D A, Point2D B) { return Vector2D(A.x - B.x, A.y - B.y); }
Vector2D operator *(Vector2D A, double p) { return Vector2D(A.x * p, A.y * p); }
Vector2D operator /(Vector2D A, double p) { return Vector2D(A.x / p, A.y / p); }

bool operator <(const Point2D& a, const Point2D& b) {
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}

const double eps = 1e-10;
int dcmp(double x) {
	if (fabs(x) < eps) return 0; 
	else return x < 0 ? -1 : 1;
}

bool operator ==(const Point2D& a, const Point2D& b) {
	return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}

double Dot(Vector2D A, Vector2D B) { 
	return A.x * B.x + A.y * B.y; 
}

double Length(Vector2D A) { 
	return sqrt(Dot(A, A)); 
}

double Angle(Vector2D A, Vector2D B) { 
	return acos(Dot(A, B) / Length(A) / Length(B)); 
}

double Cross(Vector2D A, Vector2D B) {
	return A.x * B.y - B.x * A.y; 
}

double Area2(Point2D A, Point2D B, Point2D C) { 
	return Cross(B - A, C - A); 
}

Vector2D Rotate(Vector2D A, double rad) {
	return Vector2D(A.x * cos(rad) - A.y * sin(rad), 
		A.x * sin(rad) - A.y * cos(rad));
}

Vector2D Normal(Vector2D A) {
	double L = Length(A);
	return Vector2D(-A.y / L, A.x / L);
}


Point2D GetLineIntersection(Point2D P, Vector2D v, Point2D Q, Vector2D w) {
	Vector2D u = P - Q;
	double t = Cross(w, u) / Cross(v, w);
	return P + v * t;
}
double DistanceToLine(Point2D P, Point2D A, Point2D B) {
	Vector2D v1 = B - A, v2 = P - A;
	return fabs(Cross(v1, v2) / Length(v1));
}
double DistanceToSegment(Point2D P, Point2D A, Point2D B) {
	if (A == B) return Length(P - A);
	Vector2D v1 = B - A, v2 = P - A, v3 = P - B;
	if (Dot(v1, v2) < 0) return Length(v2);
	else if (Dot(v1, v3) > 0) return Length(v3);
	else return fabs(Cross(v1, v2)) / Length(v1);
}
Point2D GetLineProjection(Point2D P, Point2D A, Point2D B) {
	Vector2D v = B - A;
	return A + v * Dot(v, P - A) / Dot(v, v);
}
bool SegmentProperIntersection(Point2D a1, Point2D a2, Point2D b1, Point2D b2) {
	double c1 = Cross(a2 - a1, b1 - a1), c2 = Cross(a2 - a1, b2 - a1);
	double c3 = Cross(b2 - b1, a1 - b1), c4 = Cross(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}
bool OnSegment(Point2D p, Point2D a1, Point2D a2) {
	return dcmp(Cross(a1 - p, a2 - p)) == 0 && dcmp(Dot(a1 - p, a2 - p)) < 0;
}

double PolygonArea(Point2D* p, int n) {
	double area = 0;
	for (int i = 1;i < n - 1;i++)
		area += Cross(p[i] - p[0], p[i + 1] - p[0]);
	return area / 2;
}

