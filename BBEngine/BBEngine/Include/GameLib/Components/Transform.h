#pragma once
#include "GameLib/BBComponent.h"
#include "Vector3.h"

class Transform : public BBComponent
{
public:
	Transform() = default;
	~Transform() = default;

	void Update(Graphics& a_Graphics) override {};
	void Draw(Graphics& a_Graphics) override {};

private:

	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	DirectX::XMMATRIX m_Transform;
};