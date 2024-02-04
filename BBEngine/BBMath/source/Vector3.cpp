#include "Vector3.h"

Vector3::Vector3(float a_X, float a_Y, float a_Z) noexcept
{
	this->x = a_X;
	this->y = a_Y;
	this->z = a_Z;
}

Vector3& Vector3::operator+(const Vector3& a_Rhs)
{
	return Vector3(
		this->x + a_Rhs.x,
		this->y + a_Rhs.x,
		this->z + a_Rhs.z);
}