#include "GameLib/BBObject.h"

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

bool BBObject::AddComponent(BBComponent* a_Component)
{
	m_Components.Push_Back(a_Component);
	return true;
}
