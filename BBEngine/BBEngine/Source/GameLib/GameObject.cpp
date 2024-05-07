#include "GameLib/GameObject.h"

namespace BBE
{
	GameObject::GameObject(Model* a_Model, Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
	{
		m_Model = a_Model;
		m_Position = a_Position;
		m_Rotation = a_Rotation;
		m_Scale = a_Scale;

		m_Model->AddToDraw(GetTransformXM());
	}

	void GameObject::Update(Graphics& a_Graphics) 
	{
		m_Model->Update(0.0f);
	}

	DirectX::XMMATRIX GameObject::GetTransformXM() const noexcept
	{
		return
			DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z) *
			DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	}

	void GameObject::SetPosition(Vector3 a_Position)
	{
		m_Position = a_Position;
	}

	Vector3 GameObject::GetPosition() const
	{
		return m_Position;
	}

	void GameObject::SetRotation(Vector3 a_Position)
	{
		m_Rotation = a_Position;
	}

	Vector3 GameObject::GetRotation() const
	{
		return m_Rotation;
	}

	void GameObject::SetScale(Vector3 a_Position)
	{
		m_Scale = a_Position;
	}

	Vector3 GameObject::GetScale() const
	{
		return m_Scale;
	}
}