#include "GameLib/GameObject.h"

namespace BBE
{
	GameObject::GameObject(Graphics& a_Graphics, Model* a_Model, Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
	{
		m_Model = a_Model;
		m_Position = a_Position;
		m_Rotation = a_Rotation;
		m_Scale = a_Scale;

		UpdateTransform();
	}

	void GameObject::Update(Graphics& a_Graphics) 
	{
		m_Model->Update(0.0f);
	}

	void GameObject::Draw(Graphics& a_Graphics)
	{
		UpdateTransform();
		m_Model->SetTransform(&m_Transform);
		m_Model->Draw(a_Graphics);
	}

	void GameObject::UpdateTransform() noexcept
	{
		DirectX::XMMATRIX objTransform = 
			DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z) *
			DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	
		m_Transform = objTransform;
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