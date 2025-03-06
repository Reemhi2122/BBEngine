#pragma once
//#include <System/BBString.h>
#include <Containers/Vector.h>
#include "Graphics.h"
#include "BBComponent.h"

constexpr auto MAX_NAME_SIZE = 255;

class BBObject
{
public:
	BBObject(const char* a_Name);
	~BBObject() = default;

	void Update(Graphics& a_Graphics);
	void Draw(Graphics& a_Graphics);
	void InspectorDrawUI();

	bool AddComponent(BBComponent* a_Component);

	const char* GetName() { return m_Name; };

private:
	char m_Name[MAX_NAME_SIZE] = "BBObject";

	//Note(Stan): Would have to make this ECS, for testing I'm just doing it like this.
	BBE::Vector<BBComponent*> m_Components;
};