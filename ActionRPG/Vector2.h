#ifndef INCLUDED_VECTOR2_H
#define INCLUDED_VECTOR2_H

class Vector2 {
public:
	Vector2();
	Vector2(double, double);
	Vector2(int, int);
	~Vector2();
	void setVector(double, double);
	void operator+=(const Vector2&);
	void operator-=(const Vector2&);
	void operator=(const Vector2&);

	double x, y;
};

#endif