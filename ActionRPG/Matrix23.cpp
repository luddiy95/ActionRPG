#include "Matrix23.h"
#include "GameLib/Math.h"

Matrix23::Matrix23(): m00(0), m01(0), m02(0), m10(0), m11(0), m12(0) {}

Matrix23::~Matrix23() {}

void Matrix23::setTranslation(const Vector2& v) {
	m00 = m11 = 1.0;
	m01 = m10 = 0.0;
	m02 = v.x;
	m12 = v.y;
}

void Matrix23::setRotation(double angle) {
	double cosine = GameLib::cos(angle);
	double sine = GameLib::sin(angle);
	m00 = cosine; m01 = -sine;
	m10 = sine; m11 = cosine;
	m02 = m12 = 0.0;
}

void Matrix23::setScaling(const Vector2& v) {
	m00 = v.x;
	m11 = v.y;
	m01 = m10 = 0.0;
	m02 = m12 = 0.0;
}

void Matrix23::operator*=(const Matrix23& v) {

	double t00 = m00, t01 = m01, t10 = m10, t11 = m11;
	m00 = t00 * v.m00 + t01 * v.m10;
	m01 = t00 * v.m01 + t01 * v.m11;
	m10 = t10 * v.m00 + t11 * v.m10;
	m11 = t10 * v.m01 + t11 * v.m11;

	m02 = t00 * v.m02 + t01 * v.m12 + m02;
	m12 = t10 * v.m02 + t11 * v.m12 + m12;

}

void Matrix23::translate(const Vector2& v) {
	m02 += m00 * v.x + m01 * v.y;
	m12 += m10 * v.x + m11 * v.y;
}

void Matrix23::rotate(double angle) {
	double sine = GameLib::sin(angle), cosine = GameLib::cos(angle);
	double tx = m00;
	m00 = tx * cosine + m01 * sine;
	m01 = -tx * sine + m01 * cosine;
	tx = m10;
	m10 = tx * cosine + m11 * sine;
	m11 = -tx * sine + m11 * cosine;
}

void Matrix23::scale(const Vector2& v) {
	m00 *= v.x;
	m01 *= v.y;
	m10 *= v.x;
	m11 *= v.y;
}

void Matrix23::multiply(Vector2* out, const Vector2& in) {
	double tx = in.x;
	out->x = m00 * tx + m01 * in.y + m02;
	out->y = m10 * tx + m11 * in.y + m12;
}