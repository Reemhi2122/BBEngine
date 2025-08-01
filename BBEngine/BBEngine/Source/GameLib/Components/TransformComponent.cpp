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

TransformComponent::TransformComponent(IGraphics& a_Graphics, Transform& a_LocalTransform, Transform& a_ObjectTransform, TransformComponent* a_ParentTransform)
{
	m_TransformBuf = new TransformBuf(a_Graphics, a_LocalTransform.GetPosition(), a_LocalTransform.GetRotationV4(), a_LocalTransform.GetScale());

	m_Position = a_ObjectTransform.GetPosition();
	m_Rotation = a_ObjectTransform.GetRotationV3();
	m_Scale = a_ObjectTransform.GetScale();

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
