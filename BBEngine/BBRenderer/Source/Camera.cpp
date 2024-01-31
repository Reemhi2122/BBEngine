#include "Camera.h"

void Camera::Update()
{
    DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
    rotation = DirectX::XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0.f);

    DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(DefaultForward, rotation);
    camTarget = DirectX::XMVector3Normalize(camTarget);

    //m_Right = DirectX::XMVector3TransformCoord(m_Right, rotation);
    //m_Forward = DirectX::XMVector3TransformCoord(m_Forward, rotation);
    //m_Up = DirectX::XMVector3TransformCoord(m_Up, rotation);

    DirectX::XMMATRIX RotateYTempMatrix;
    RotateYTempMatrix = DirectX::XMMatrixRotationY(camYaw);

    m_Right = DirectX::XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
    m_Up = DirectX::XMVector3TransformCoord(m_Up, RotateYTempMatrix);
    m_Forward = DirectX::XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Right, camRightMove));
    m_Position = DirectX::XMVectorAdd(m_Position, DirectX::XMVectorScale(m_Forward, camForwardMove));

    camForwardMove = 0.0f;
    camRightMove = 0.0f;

    //camYaw = 0.0f;
    //camPitch = 0.0f;

    camTarget = DirectX::XMVectorAdd(m_Position, camTarget);
    m_ViewMatrix = DirectX::XMMatrixLookToLH(m_Position, camTarget, m_Up);
}
