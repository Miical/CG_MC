#ifndef VECTOR2D_H
#define VECTOR2D_H

struct Point2D {
	double x, y;
	Point2D(double x = 0, double y = 0) :x(x), y(y) {}
};
typedef Point2D Vector2D;

Vector2D operator +(Vector2D A, Vector2D B);
Vector2D operator -(Point2D A, Point2D B);
Vector2D operator *(Vector2D A, double p);
Vector2D operator /(Vector2D A, double p);

double Dot(Vector2D A, Vector2D B);
double Length(Vector2D A);
double Angle(Vector2D A, Vector2D B);
double Cross(Vector2D A, Vector2D B);
double Area2(Point2D A, Point2D B, Point2D C);

#endif
