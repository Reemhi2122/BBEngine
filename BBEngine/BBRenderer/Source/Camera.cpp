#include "Camera.h"

void Camera::Update()
{
    DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
    rotation = DirectX::XMMatrixRotationRollPitchYaw(camRoll, camPitch, 0.f);

    m_Right = DirectX::XMVector3TransformCoord(m_Right, rotation);
    m_Forward = DirectX::XMVector3TransformCoord(m_Forward, rotation);
    m_Up = DirectX::XMVector3TransformCoord(m_Up, rotation);

    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Forward, camForwardMove));
    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Right, camRightMove));

    camForwardMove = 0.0f;
    camRightMove = 0.0f;

    camRoll = 0.0f;
    camPitch = 0.0f;

    m_ViewMatrix = DirectX::XMMatrixLookToLH(m_Position, m_Forward, m_Up);
}
