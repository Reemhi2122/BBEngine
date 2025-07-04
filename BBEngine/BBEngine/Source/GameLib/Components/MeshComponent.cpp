#include "GameLib/Components/MeshComponent.h"

namespace BBE
{
	MeshComponent::MeshComponent(IGraphics& a_Graphics, Model* a_Model, uint32_t a_NodeIndex, Transform* a_Transform)
	{
		m_Transform = a_Transform;
		m_Model = a_Model;
		m_NodeIndex = a_NodeIndex;
	}

	void MeshComponent::Update(IGraphics& a_Graphics)
	{
		m_Model->Update(0.0f);
	}

	void MeshComponent::Draw(IGraphics& a_Graphics)
	{
		m_Model->SetTransform(m_Transform->GetTransformBuf());
		m_Model->Draw(a_Graphics, m_NodeIndex);
	}

	bool MeshComponent::SetModel(Model* a_Model)
	{
		m_Model = a_Model;
		return true;
	}

	void MeshComponent::InspectorDrawUI()
	{
		if (ImGui::TreeNode("MeshComponent"))
		{
			//Note(Stan): Testing some ImGui functionality
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
			static int item_current = 0;
			ImGui::Combo("Mesh", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::TreePop();
		}
	}
}