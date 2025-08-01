#pragma once
#include <System/BBString.h>
#include <Containers/Vector.h>
#include "SharedRenderTypes.h"
#include "Graphics.h"
#include "BBComponent.h"

class Model;

//Note(Stan): Make UUID with actual unique number.
static uint32_t OBJECT_UUID = 0;
#define UUID OBJECT_UUID++

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

	std::vector<BBObject*>* GetChildren() { return &m_Children; };

	const uint32_t GetUUID() { return m_UUID; };
	const char* GetName() { return m_Name; };

	static void CreateObjectsFromModel(IGraphics& a_Gfx, Model* a_Model, const BBE::GLTFFile* a_GLTFFile, std::vector<BBObject*>* a_AllObjects, std::vector<BBObject*>* a_RootObjects, Vector3 a_Pos, Vector3 a_Rot, Vector3 a_Scale);

protected:
	//Note(Stan): For now set child is connected to support a unified way of setting parent / child relation.
	bool SetChild(BBObject* a_Child);

private:

	char m_Name[BBE::MAX_NAME_LENGTH] = "BBObject";

	uint32_t m_UUID = 0;

	//Note(Stan): Would have to make this ECS, for testing I'm just doing it like this.
	BBE::Vector<BBComponent*> m_Components;

	BBObject* m_ParentObject = nullptr;
	std::vector<BBObject*> m_Children;
};