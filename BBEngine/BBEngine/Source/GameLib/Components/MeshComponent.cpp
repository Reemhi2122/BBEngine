#include "GameLib/Components/MeshComponent.h"

namespace BBE
{
	MeshComponent::MeshComponent(Graphics& a_Graphics, Model* a_Model, Transform* a_Transform)
	{
		m_Transform = a_Transform;
		m_Model = a_Model;
	}

	void MeshComponent::Update(Graphics& a_Graphics)
	{
		m_Model->Update(0.0f);
	}

	void MeshComponent::Draw(Graphics& a_Graphics)
	{
		m_Model->SetTransform(&m_Transform->GetTransform());
		m_Model->Draw(a_Graphics);
	}

	void MeshComponent::InspectorDrawUI()
	{
		if (ImGui::TreeNode("MeshComponent"))
		{
			ImGui::TreePop();
		}
	}
}