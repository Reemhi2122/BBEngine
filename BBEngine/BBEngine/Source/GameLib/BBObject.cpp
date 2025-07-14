#include "GameLib/BBObject.h"
#include "GameLib/Drawable/Model.h"
#include "GameLib/Components/MeshComponent.h"
#include "GameLib/Components/Transform.h"

BBObject::BBObject(const char* a_Name)
{
	strcpy(m_Name, a_Name);
	m_UUID = UUID;
}

void BBObject::Update(Graphics& a_Graphics)
{
	for (uint32_t i = 0; i < m_Components.Size(); i++)
	{
		m_Components[i]->Update(a_Graphics);
	}
}

void BBObject::Draw(Graphics& a_Graphics)
{
	for (uint32_t i = 0; i < m_Components.Size(); i++)
	{
		m_Components[i]->Draw(a_Graphics);
	}
}

void BBObject::InspectorDrawUI()
{
	ImGui::Text(m_Name);
	ImGui::Spacing();
	ImGui::Indent();

	ImGui::InputText("Object Name", m_Name, BBE::MAX_NAME_LENGTH);
	for (uint32_t i = 0; i < m_Components.Size(); i++)
	{
		m_Components[i]->InspectorDrawUI();
	}
}

bool BBObject::AddComponent(BBComponent* a_Component)
{
	m_Components.Push_Back(a_Component);
	return true;
}

bool BBObject::SetParent(BBObject* a_Parent)
{
	m_ParentObject = a_Parent;
	m_ParentObject->SetChild(this);
	return true;
}

bool BBObject::SetChild(BBObject* a_Child)
{
	m_Children.push_back(a_Child);
	return true;
}

void BBObject::CreateObjectsFromModel(IGraphics& a_Gfx, Model* a_Model, std::vector<BBObject*>* a_AllObjects, std::vector<BBObject*>* a_RootObjects, Vector3 a_Pos, Vector3 a_Rot, Vector3 a_Scale)
{
	NodeContainer nodes = a_Model->GetNodes();

	BBObject* parentObj = nullptr;
	Transform* parentTransformComp = nullptr;
	if(nodes.count > 0)
	{
		parentObj = new BBObject("Parent Node");
		parentTransformComp = new Transform(a_Gfx, a_Pos, a_Rot, a_Scale);
		parentObj->AddComponent(parentTransformComp);
		a_AllObjects->push_back(parentObj);
		a_RootObjects->push_back(parentObj);
	}

	for (uint32_t nodeIndex = 0; nodeIndex < nodes.count; nodeIndex++)
	{
		BBObject* obj = new BBObject("test");
		Transform* TransformComp = new Transform(a_Gfx, &nodes.data[nodeIndex], parentTransformComp);
		BBE::MeshComponent* MeshComp = new BBE::MeshComponent(a_Gfx, a_Model, nodeIndex, TransformComp);
		obj->AddComponent(TransformComp);
		obj->AddComponent(MeshComp);

		obj->SetParent(parentObj);
		a_AllObjects->push_back(obj);
	}
}