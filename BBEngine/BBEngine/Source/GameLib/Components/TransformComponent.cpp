#include "GameLib/Components/TransformComponent.h"
#include "GameLib/Drawable/Model.h"

TransformComponent::TransformComponent(IGraphics& a_Graphics, Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
{
	m_TransformBuf = new TransformBuf(a_Graphics, m_Position, Vector4(0, 0, 0, 0), m_Scale);

	m_Position = a_Position;
	m_Rotation = a_Rotation;
	m_Scale = a_Scale;

	m_TransformBuf->SetTransformRPY(TransformType::OBJECT, m_Position, m_Rotation, m_Scale);
}

TransformComponent::TransformComponent(IGraphics& a_Graphics, ModelTransform* a_Transform, TransformComponent* a_ParentTransform)
{
	m_TransformBuf = new TransformBuf(a_Graphics, a_Transform->position, a_Transform->rotation, a_Transform->scale);

	m_Position = Vector3(0, 0, 0);
	m_Rotation = Vector3(0, 0, 0);
	m_Scale = Vector3(1, 1, 1);

	if (a_ParentTransform)
	{
		m_ParentTransform = a_ParentTransform;
		m_TransformBuf->SetTransformRPY(TransformType::PARENT, m_ParentTransform->GetPosition(), m_ParentTransform->GetRotation(), m_ParentTransform->GetScale());
	}

	m_TransformBuf->SetTransformRPY(TransformType::OBJECT, m_Position, m_Rotation, m_Scale);
}

void TransformComponent::Update(IGraphics& a_Graphics)
{
	m_TransformBuf->SetTransformRPY(TransformType::OBJECT, m_Position, m_Rotation, m_Scale);

	if (m_ParentTransform)
	{
		m_TransformBuf->SetTransformRPY(TransformType::PARENT, m_ParentTransform->GetPosition(), m_ParentTransform->GetRotation(), m_ParentTransform->GetScale());
	}
}

void TransformComponent::InspectorDrawUI()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::InputFloat3("Position", m_Position.GetXYZ());
		ImGui::InputFloat3("Rotation", m_Rotation.GetXYZ());
		ImGui::InputFloat3("Scale", m_Scale.GetXYZ());
		ImGui::TreePop();
	}
}
