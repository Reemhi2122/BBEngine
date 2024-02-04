#pragma once

class Vector3
{
public:
	Vector3() noexcept = default;
	Vector3(float, float, float) noexcept;
	~Vector3() noexcept = default;

	Vector3& operator+(const Vector3& a_Rhs);

public:
	float 
		x = 0, 
		y = 0, 
		z = 0;

private:
};