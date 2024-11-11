#pragma once

class Vector4 {
public:
	Vector4();
	Vector4(float a_X, float a_Y, float a_Z, float a_W);
	~Vector4() = default;

	float* GetXYZ() { return xyz; };

	Vector4& operator+(const Vector4& a_Rhs);
	Vector4& operator=(const Vector4& a_Rhs);
	Vector4& operator*(const Vector4& a_Rhs);

public:
	union
	{
		float vector[4] = { 0, 0, 0, 0};
		float xyz[3];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};

};