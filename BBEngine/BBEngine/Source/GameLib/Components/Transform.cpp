#include "GameLib/Components/Transform.h"
#include "GameLib/Drawable/Model.h"

Transform::Transform(IGraphics& a_Graphics, Vector3 a_Position, Vector3 a_Rotation, Vector3 a_Scale)
{
	m_TransformBuf = new TransformBuf(a_Graphics, m_Position, Vector4(0, 0, 0, 0), m_Scale);

	m_Position = a_Position;
	m_Rotation = a_Rotation;
	m_Scale = a_Scale;

	m_TransformBuf->SetTransformRPY(TransformType::OBJECT, m_Position, m_Rotation, m_Scale);
}

Transform::Transform(IGraphics& a_Graphics, ModelNodes* a_Model, Transform* a_ParentTransform)
{
	ModelTransform modTransform = a_Model->objectTransform;
	m_TransformBuf = new TransformBuf(a_Graphics, modTransform.position, modTransform.rotation, modTransform.scale);

	m_Position = Vector3(0, 0, 0);
	m_Rotation = Vector3(0, 0, 0);
	m_Scale = Vector3(1, 1, 1);

	if (a_ParentTransform)
	{
		m_TransformBuf->SetTransformRPY(TransformType::PARENT, a_ParentTransform->GetPosition(), a_ParentTransform->GetRotation(), a_ParentTransform->GetScale());
	}

	m_TransformBuf->SetTransformRPY(TransformType::OBJECT, m_Position, m_Rotation, m_Scale);
}

void Transform::Update(IGraphics& a_Graphics)
{
	m_TransformBuf->SetTransformRPY(TransformType::OBJECT, m_Position, m_Rotation, m_Scale);
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
