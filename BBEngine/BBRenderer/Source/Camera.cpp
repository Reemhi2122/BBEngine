#include "Camera.h"

void Camera::Update()
{
    DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
    rotation = m_BaseRotation * DirectX::XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0.f);
    DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(DefaultForward, rotation);
    camTarget = DirectX::XMVector3Normalize(camTarget);
    
    DirectX::XMMATRIX RotateYTempMatrix;
    RotateYTempMatrix = DirectX::XMMatrixRotationY(camYaw);

    m_Right = DirectX::XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
    m_Up = DirectX::XMVector3TransformCoord(m_Up, RotateYTempMatrix);
    m_Forward = DirectX::XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Right, camRightMove));
    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Forward, camForwardMove));
    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Up, camUpMove));

    camForwardMove = 0.0f;
    camRightMove = 0.0f;
    camUpMove = 0.0f;

    camTarget = DirectX::XMVectorAdd(m_Position, camTarget);
    m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, camTarget, m_Up);
}
