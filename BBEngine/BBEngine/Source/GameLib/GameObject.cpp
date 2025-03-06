#include "GameLib/GameObject.h"

namespace BBE
{
	GameObject::GameObject(Graphics& a_Graphics, Model* a_Model, Transform* a_Transform)
	{
		m_Transform = a_Transform;
		m_Model = a_Model;
	}

	void GameObject::Update(Graphics& a_Graphics) 
	{
		m_Model->Update(0.0f);
	}

	void GameObject::Draw(Graphics& a_Graphics)
	{
		m_Model->SetTransform(&m_Transform->GetTransform());
		m_Model->Draw(a_Graphics);
	}

	void GameObject::InspectorDrawUI()
	{
		if (ImGui::TreeNode("GameObject"))
		{
			ImGui::Text("GameObject");
			ImGui::TreePop();
		}
	}
}