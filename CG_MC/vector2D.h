#ifndef VECTOR2D_H
#define VECTOR2D_H

struct Point2D {
	float x, y;
	Point2D(float x = 0, float y = 0) :x(x), y(y) {}
};
typedef Point2D Vector2D;

Vector2D operator +(Vector2D A, Vector2D B);
Vector2D operator -(Point2D A, Point2D B);
Vector2D operator *(Vector2D A, float p);
Vector2D operator /(Vector2D A, float p);

float Dot(Vector2D A, Vector2D B);
float Length(Vector2D A);
float Angle(Vector2D A, Vector2D B);
float Cross(Vector2D A, Vector2D B);
float Area2(Point2D A, Point2D B, Point2D C);

#endif
