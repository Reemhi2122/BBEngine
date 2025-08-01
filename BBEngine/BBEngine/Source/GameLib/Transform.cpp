#include "Gamelib/Transform.h"

Transform::Transform(Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
{
	m_position = a_Position;
	m_V3Rotation = a_Rotation;
	m_Scale = a_Scale;
}

Transform::Transform(Vector3 a_Position, Vector4 a_Rotation, Vector3 a_Scale)
{
	m_position = a_Position;
	m_V4Rotation = a_Rotation;
	m_Scale = a_Scale;
}