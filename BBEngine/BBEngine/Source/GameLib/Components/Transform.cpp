#include "GameLib/Components/Transform.h"

Transform::Transform(Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
{
	m_Position = a_Position;
	m_Rotation = a_Rotation;
	m_Scale = a_Scale;

	m_Transform = DirectX::XMMatrixIdentity();
}

void Transform::Update(Graphics& a_Graphics)
{
	DirectX::XMMATRIX objTransform =
		DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z) *
		DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_Transform = objTransform;
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
