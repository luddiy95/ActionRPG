#include "Vector2.h"

Vector2::Vector2() : x(0), y(0) {};

Vector2::Vector2(int ax, int ay) {
	x = static_cast<double>(ax);
	y = static_cast<double>(ay);
}

Vector2::Vector2(double ax, double ay) : x(ax), y(ay) {}

Vector2::~Vector2() {

}

void Vector2::setVector(double ax, double ay) {
	x = ax; 
	y = ay;
}

void Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}

void Vector2::operator=(const Vector2& v) {
	x = v.x;
	y = v.y;
}