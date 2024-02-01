#pragma once
#include <DirectXMath.h>

class Camera {
public:
	DirectX::XMMATRIX GetViewMatrix() const;
	void SetViewMatrix(DirectX::XMMATRIX& a_ViewMatrix);

	DirectX::XMVECTOR GetPosition() const;
	void SetPosition(DirectX::XMVECTOR& a_Position);

	DirectX::XMMATRIX GetRotation() const;
	void SetRotation(DirectX::XMMATRIX& a_Rotation);

	void Update();

public:
	float camYaw = 0.0f;
	float camPitch = 0.0f;

	float camForwardMove = 0.0f;
	float camRightMove = 0.0f;
	float camUpMove = 0.0f;

private:
	DirectX::XMVECTOR DefaultRight =	DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultForward =	DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR m_Right =		DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	DirectX::XMVECTOR m_Up =		DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	DirectX::XMVECTOR m_Forward =	DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);

	DirectX::XMVECTOR m_Position =	DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.f);
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

