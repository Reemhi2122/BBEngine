#include "EntityManager.h"

namespace ECS {
	EntityManager::EntityManager() {
		for (Entity i = 0; i < MAXENTITIES; i++)
			m_AvailableEntities.Push(i);

		m_ExistingEntities = 0;
	}

	EntityManager::~EntityManager() {}

	Entity EntityManager::CreateEntity()
	{
		if (m_AvailableEntities.IsEmpty()) 
			throw std::runtime_error("ECS || There are no more available entities.");

		Entity t_Entity = 0;
		m_AvailableEntities.Pop(&t_Entity);
		m_ExistingEntities++;

		return t_Entity;
	}

	bool EntityManager::DeleteEntity(Entity a_TargetEntity)
	{
		if (a_TargetEntity > MAXENTITIES)
			throw std::runtime_error("ECS || Entity out of range.");

		m_Signatures[a_TargetEntity].reset();
		m_AvailableEntities.Push(a_TargetEntity);
		m_ExistingEntities--;

		return true;
	}

	Signature EntityManager::GetSignature(Entity a_TargetEntity)
	{
		if(a_TargetEntity > MAXENTITIES)
			throw std::runtime_error("ECS ||Entity out of range.");
		
		return m_Signatures[a_TargetEntity];
	}

	void EntityManager::SetSignature(Entity a_TargetEntity, Signature a_Signature)
	{
		if(a_TargetEntity > MAXENTITIES)
			throw std::runtime_error("ECS ||Entity out of range.");

		m_Signatures[a_TargetEntity] = a_Signature;
	}
}