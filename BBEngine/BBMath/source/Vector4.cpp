#include "Vector4.h"

Vector4::Vector4() {
	x = y = z = w = 0;
}

Vector4::Vector4(float a_X, float a_Y, float a_Z, float a_W)
	: x(a_X), y(a_Y), z(a_Z), w(a_W)
{}

Vector4::~Vector4()
{}

Vector4& Vector4::operator+(const Vector4& a_Rhs) 
{	
	if (this == &a_Rhs) {
		return *this;
	}

	x += a_Rhs.x;
	y += a_Rhs.y;
	z += a_Rhs.z;
	w += a_Rhs.w;

	return *this;
}

Vector4& Vector4::operator=(const Vector4& a_Rhs) 
{
	if (this == &a_Rhs) {
		return *this;
	}

	x = a_Rhs.x;
	y = a_Rhs.y;
	z = a_Rhs.z;
	w = a_Rhs.w;

	return *this;
}