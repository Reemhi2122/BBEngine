#pragma once
#include "BBMath.h"

class Transform
{
public:
	Transform(Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale);
	Transform(Vector3 a_Position, Vector4 a_Rotation, Vector3 a_Scale);

	Vector3 GetPosition() const noexcept { return m_position; };
	Vector3 GetRotationV3() const noexcept { return m_V3Rotation; };
	Vector4 GetRotationV4() const noexcept { return m_V4Rotation; };
	Vector3 GetScale() const noexcept { return m_Scale; };

private:
	BBMath::Matrix4x4 m_Transform;

	//Note(Stan): Currenlty using V3 / V4 rotations as I don't want to convert
	//				it to a matrix to get these.
	Vector3 m_position;
	Vector3 m_V3Rotation;
	Vector4 m_V4Rotation;
	Vector3 m_Scale;
};