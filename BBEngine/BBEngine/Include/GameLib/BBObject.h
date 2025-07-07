#pragma once
#include <System/BBString.h>
#include <Containers/Vector.h>
#include "Graphics.h"
#include "BBComponent.h"

class Model;

class BBObject
{
public:
	BBObject(const char* a_Name, BBObject* a_ParentObject);
	~BBObject() = default;

	void Update(Graphics& a_Graphics);
	void Draw(Graphics& a_Graphics);
	void InspectorDrawUI();

	bool AddComponent(BBComponent* a_Component);

	const char* GetName() { return m_Name; };

	static void CreateObjectsFromModel(IGraphics& a_Gfx, Model* a_Model, std::vector<BBObject*>* m_Objects, Vector3 pos, Vector3 rot, Vector3 scale);

private:
	char m_Name[BBE::MAX_NAME_LENGTH] = "BBObject";

	//Note(Stan): Would have to make this ECS, for testing I'm just doing it like this.
	BBE::Vector<BBComponent*> m_Components;

	BBObject* m_ParentObject;
};