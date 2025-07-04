#include "GameLib/BBObject.h"
#include "GameLib/Drawable/Model.h"
#include "GameLib/Components/MeshComponent.h"
#include "GameLib/Components/Transform.h"

BBObject::BBObject(const char* a_Name)
{
	strcpy(m_Name, a_Name);
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

void BBObject::CreateObjectsFromModel(IGraphics& a_Gfx, Model* a_Model, std::vector<BBObject*>* m_Objects, Vector3 pos, Vector3 rot, Vector3 scale)
{
	NodeContainer nodes = a_Model->GetNodes();
	for (uint32_t nodeIndex = 0; nodeIndex < nodes.count; nodeIndex++)
	{
		BBObject* obj = new BBObject("test");
		Transform* sponzaTransform = new Transform(a_Gfx, &nodes.data[nodeIndex], pos, rot, scale);
		BBE::MeshComponent* sponzaMesh = new BBE::MeshComponent(a_Gfx, a_Model, nodeIndex, sponzaTransform);
		obj->AddComponent(sponzaTransform);
		obj->AddComponent(sponzaMesh);
		m_Objects->push_back(obj);
	}
}