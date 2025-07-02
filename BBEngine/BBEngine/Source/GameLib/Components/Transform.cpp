#include "GameLib/Components/Transform.h"
#include "GameLib/Drawable/Model.h"

Transform::Transform(IGraphics& a_Graphics, Model* a_Model, Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
{
	ModelTransform modTransform = a_Model->GetNodes().data[0].objectTransform;
	m_TransformBuf = new TransformBuf(a_Graphics, modTransform.position, modTransform.rotation, modTransform.scale);
	
	m_Position = a_Position;
	m_Rotation = a_Rotation;
	m_Scale = a_Scale;

	m_TransformBuf->SetObjectTransform(m_Position, m_Rotation, m_Scale);
}

void Transform::Update(IGraphics& a_Graphics)
{
	m_TransformBuf->SetObjectTransform(m_Position, m_Rotation, m_Scale);
}

void Transform::InspectorDrawUI()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::InputFloat3("Position", m_Position.GetXYZ());
		ImGui::InputFloat3("Rotation", m_Rotation.GetXYZ());
		ImGui::InputFloat3("Scale", m_Scale.GetXYZ());
		ImGui::TreePop();
	}
}
