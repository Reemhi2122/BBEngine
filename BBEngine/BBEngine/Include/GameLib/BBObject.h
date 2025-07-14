#pragma once
#include <System/BBString.h>
#include <Containers/Vector.h>
#include "Graphics.h"
#include "BBComponent.h"

class Model;

class BBObject
{
public:
	BBObject(const char* a_Name);
	~BBObject() = default;

	void Update(Graphics& a_Graphics);
	void Draw(Graphics& a_Graphics);
	void InspectorDrawUI();

	bool AddComponent(BBComponent* a_Component);
	bool SetParent(BBObject* a_Parent);

	bool SetChild(BBObject* a_Child);
	std::vector<BBObject*>* GetChildren() { return &m_Children; };

	const char* GetName() { return m_Name; };

	static void CreateObjectsFromModel(IGraphics& a_Gfx, Model* a_Model, std::vector<BBObject*>* a_AllObjects, std::vector<BBObject*>* a_RootObjects, Vector3 a_Pos, Vector3 a_Rot, Vector3 a_Scale);

private:
	char m_Name[BBE::MAX_NAME_LENGTH] = "BBObject";

	//Note(Stan): Would have to make this ECS, for testing I'm just doing it like this.
	BBE::Vector<BBComponent*> m_Components;

	BBObject* m_ParentObject = nullptr;
	std::vector<BBObject*> m_Children;
};