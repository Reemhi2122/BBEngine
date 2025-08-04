#include "GameLib/BBObject.h"
#include "GameLib/Drawable/Model.h"
#include "GameLib/Components/MeshComponent.h"
#include "GameLib/Components/TransformComponent.h"

BBObject::BBObject(const char* a_Name)
{
	strcpy(m_Name, a_Name);
	m_UUID = UUID;
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

bool BBObject::SetParent(BBObject* a_Parent)
{
	m_ParentObject = a_Parent;
	m_ParentObject->SetChild(this);
	return true;
}

bool BBObject::SetChild(BBObject* a_Child)
{
	assert(false, "Not supported for now!");
	m_Children.push_back(a_Child);
	return true;
}

void BBObject::CreateObjectsFromModel(IGraphics& a_Gfx, Model* a_Model, const BBE::GLTFFile* a_GLTFFile, std::vector<BBObject*>* a_AllObjects, std::vector<BBObject*>* a_RootObjects, Transform& a_ObjectTransform)
{
	BBObject* parentObj = nullptr;

	//Parent node

	//Children node

	//Bind children to parent

	for (uint32_t nodeIndex = 0; nodeIndex < a_GLTFFile->nodeAmount; nodeIndex++)
	{
		BBE::GLTFNode* curNode = &a_GLTFFile->nodes[nodeIndex];
		BBObject* obj = new BBObject(curNode->name);
		Transform localTransform = Transform(curNode->translation, curNode->rotation, curNode->scale);
		TransformComponent* TransformComp = new TransformComponent(a_Gfx, localTransform, a_ObjectTransform, nullptr);
		BBE::MeshComponent* MeshComp = new BBE::MeshComponent(a_Gfx, a_Model, nodeIndex, TransformComp);
		obj->AddComponent(TransformComp);
		obj->AddComponent(MeshComp);

		a_AllObjects->push_back(obj);
	}
}