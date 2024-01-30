#pragma once
#include <DirectXMath.h>

class Camera {
public:
	DirectX::XMMATRIX GetViewMatrix() const;
	void SetViewMatrix(DirectX::XMMATRIX& a_ViewMatrix);

	DirectX::XMVECTOR GetPosition() const;
	void SetPosition(DirectX::XMVECTOR& a_Position);

	DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	DirectX::XMVECTOR right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

private:
	DirectX::XMVECTOR m_Position = DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.f);
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMatrixIdentity();
};

inline DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

inline void Camera::SetViewMatrix(DirectX::XMMATRIX& a_ViewMatrix)
{
	m_ViewMatrix = a_ViewMatrix;
}

inline DirectX::XMVECTOR Camera::GetPosition() const
{
	return m_Position;
}

inline void Camera::SetPosition(DirectX::XMVECTOR& a_Position)
{
	m_Position = a_Position;
}