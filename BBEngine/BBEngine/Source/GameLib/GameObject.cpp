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
		m_Model->SetPosition(m_Position);
		m_Model->SetRotation(m_Rotation);
		m_Model->SetScale(m_Scale);
		m_Model->Update(0.0f);
		m_Model->Draw(a_Graphics);
	}
}