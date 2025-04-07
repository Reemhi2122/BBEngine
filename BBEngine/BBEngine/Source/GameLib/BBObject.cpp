#include "GameLib/BBObject.h"

BBObject::BBObject(const char* a_Name, bool a_InitializeDefault)
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
