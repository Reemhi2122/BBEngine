#include "GameLib/GameObject.h"

namespace BBE
{
	GameObject::GameObject(Model* a_Model)
	{
		m_Model = a_Model;
	}

	void GameObject::Update(Graphics& a_Graphics) 
	{
		m_Model->Update(0.0f);
		m_Model->Draw(a_Graphics);
	}
}