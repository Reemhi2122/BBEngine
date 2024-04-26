#pragma once

class Vector4 {
public:
	Vector4();
	Vector4(float a_X, float a_Y, float a_Z, float a_W);
	~Vector4() = default;

	Vector4& operator+(const Vector4& a_Rhs);
	Vector4& operator=(const Vector4& a_Rhs);
	Vector4& operator*(const Vector4& a_Rhs);

public:
	float x, y, z, w;

};