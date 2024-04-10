#include "GameLib/GameObject.h"

namespace BBE
{
	GameObject::GameObject(Model* a_Model, Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
	{
		m_Model = a_Model;
		m_Position = a_Position;
		m_Rotation = a_Rotation;
		m_Scale = a_Scale;
	}

	void GameObject::Update(Graphics& a_Graphics) 
	{
		m_Model->Update(0.0f);
		m_Model->Draw(a_Graphics);
	}

	DirectX::XMMATRIX GameObject::GetTransformXM() const noexcept
	{
		return
			DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z) *
			DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	}
}