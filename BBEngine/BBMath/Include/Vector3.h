#pragma once

class Vector3
{
public:
	Vector3() noexcept = default;
	Vector3(float, float, float) noexcept;
	~Vector3() noexcept = default;

	float* GetXYZ() { return vector; };

	Vector3& operator+(const Vector3& a_Rhs);
	Vector3& operator-(const Vector3& a_Rhs);

public:
	union
	{
		float vector[3] = {0, 0, 0};
		struct
		{
			float x;
			float y;
			float z;
		};
	};

private:
};