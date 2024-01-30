#pragma once
#include <DirectXMath.h>

class Camera {

public:
	DirectX::XMMATRIX GetPos() const;
	void SetPos(DirectX::XMMATRIX& a_Position);

public:
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	DirectX::XMVECTOR right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DirectX::XMVECTOR position = DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.f);

	DirectX::XMMATRIX m_Position = DirectX::XMMatrixIdentity();
};

inline DirectX::XMMATRIX Camera::GetPos() const
{
	return m_Position;
}

inline void Camera::SetPos(DirectX::XMMATRIX& a_Position)
{
	m_Position = a_Position;
}