#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class Camera {
public:
	Camera();
	~Camera() = default;

	DirectX::XMMATRIX GetViewMatrix() const;
	void SetViewMatrix(DirectX::XMMATRIX& a_ViewMatrix);

	DirectX::XMVECTOR GetPosition() const;
	void SetPosition(DirectX::XMVECTOR& a_Position);

	DirectX::XMMATRIX GetRotation() const;
	void SetRotation(DirectX::XMMATRIX& a_Rotation);

	void SetViewPort(float a_ViewPortWidth, float a_ViewPortHeight);
	D3D11_VIEWPORT* GetViewPort() { return &m_Viewport; };

	void SetProjection(DirectX::XMMATRIX a_Projections);
	DirectX::XMMATRIX GetProjection() const noexcept;

	void Update();

	float camYaw = 0.0f;
	float camPitch = 0.0f;

	float camForwardMove = 0.0f;
	float camRightMove = 0.0f;
	float camUpMove = 0.0f;

public:
	DirectX::XMVECTOR DefaultRight =	DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultForward =	DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR m_Right =		DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	DirectX::XMVECTOR m_Up =		DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	DirectX::XMVECTOR m_Forward =	DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);

	DirectX::XMVECTOR m_Position =	DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.f);
	DirectX::XMMATRIX m_BaseRotation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX m_Projection = DirectX::XMMatrixIdentity();

private:
	D3D11_VIEWPORT m_Viewport;
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

inline DirectX::XMMATRIX Camera::GetRotation() const {
	return m_BaseRotation;
}

inline void Camera::SetRotation(DirectX::XMMATRIX& a_Rotation) {
	m_BaseRotation = a_Rotation;
}

inline void Camera::SetProjection(DirectX::XMMATRIX a_Projections)
{
	m_Projection = a_Projections;
}

inline DirectX::XMMATRIX Camera::GetProjection() const noexcept
{
	return m_Projection;
}
