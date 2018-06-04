#ifndef INCLUDED_MATRIX23_H
#define INCLUDED_MATRIX23_H

#include "Vector2.h"

class Matrix23 {
public:
	Matrix23();
	~Matrix23();
	void setTranslation(const Vector2&);
	void setRotation(double);
	void setScaling(const Vector2&);
	void translate(const Vector2&);
	void rotate(double);
	void scale(const Vector2&);
	void operator*=(const Matrix23&);
	void multiply(Vector2*, const Vector2&);
private:
	double m00, m01, m02, m10, m11, m12;
};

#endif