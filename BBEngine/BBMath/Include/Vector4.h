#pragma once

class Vector4 {
	Vector4();
	Vector4(float a_X, float a_Y, float a_Z, float a_W);
	~Vector4();

	Vector4& operator+(const Vector4& a_Rhs);
	Vector4& operator=(const Vector4& a_Rhs);
	Vector4& operator*(const Vector4& a_Rhs);

private:
	float x, y, z, w;

};