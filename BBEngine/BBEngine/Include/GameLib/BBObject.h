#pragma once
#include <System/BBString.h>
#include <Containers/LinkedList.h>

class Component;

class BBObject
{
public:
	BBObject() = default;
	~BBObject() = default;

	virtual void Update(Graphics& a_Graphics) = 0;

	bool AddComponent(Component* a_Component);

private:
	BBE::BBString m_Name;

	//Note(Stan): Would have to make this ECS, for testing I'm just doing it like this.
	BBE::LinkedList<Component*> m_Components; 
};