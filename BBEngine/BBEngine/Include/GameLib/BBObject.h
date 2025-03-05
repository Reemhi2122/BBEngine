#pragma once
//#include <System/BBString.h>
#include <Containers/LinkedList.h>
#include "Graphics.h"
#include "BBComponent.h"

class BBObject
{
public:
	BBObject() = default;
	~BBObject() = default;

	void Update(Graphics& a_Graphics);
	void Draw(Graphics& a_Graphics);

	bool AddComponent(BBComponent* a_Component);

	const char* GetName() { return m_Name; };

private:
	//BBE::BBString m_Name;

	char* m_Name = "BBObject";

	//Note(Stan): Would have to make this ECS, for testing I'm just doing it like this.
	BBE::LinkedList<BBComponent*> m_Components;
};