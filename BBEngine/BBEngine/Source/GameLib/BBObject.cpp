#include "GameLib/BBObject.h"

bool BBObject::AddComponent(Component* a_Component)
{
	m_Components.Push_Back(a_Component);
	return true;
}
