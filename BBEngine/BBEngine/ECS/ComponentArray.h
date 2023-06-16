#pragma once

#include <array>
#include <unordered_map>

#include "ECSInclude.h"

namespace ECS {

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity a_TargetEntity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		ComponentArray() {};
		~ComponentArray() {};

		bool InsertComponent(Entity a_TargetEntity, T a_Component)
		{
			uint32_t t_Index = m_Size;
			m_EntityToIndexMap[a_TargetEntity] = t_Index;
			m_IndexToEntityMap[t_Index] = a_TargetEntity;
			m_ComponentList[t_Index] = a_Component;
			m_Size++;

			return true;
		}

		bool RemoveData(Entity a_TargetEntity)
		{
			if (m_EntityToIndexMap.find(a_TargetEntity) == m_EntityToIndexMap.end())
				throw std::runtime_error("Trying to get a non existent component");

			uint32_t t_RemoveIndex = m_EntityToIndexMap[a_TargetEntity];
			uint32_t t_ReplaceIndex = m_Size - 1;
			m_ComponentList[t_RemoveIndex] = m_ComponentList[t_ReplaceIndex];

			Entity t_ReplacedEntity = m_IndexToEntityMap[t_ReplaceIndex];
			m_EntityToIndexMap[t_ReplacedEntity] = t_RemoveIndex;
			m_IndexToEntityMap[t_RemoveIndex] = t_ReplacedEntity;

			m_EntityToIndexMap.erase(a_TargetEntity);
			m_IndexToEntityMap.erase(t_ReplaceIndex);
			m_Size--;

			return true;
		}

		T& GetComponent(Entity a_TargetEntity)
		{
			if(m_EntityToIndexMap.find(a_TargetEntity) == m_EntityToIndexMap.end())
				throw std::runtime_error("Trying to get a non existent component");
			
			return m_ComponentList[m_EntityToIndexMap[a_TargetEntity]];
		}

		void EntityDestroyed(Entity a_TargetEntity) override
		{
			if (m_EntityToIndexMap.find(a_TargetEntity) != m_EntityToIndexMap.end())
				RemoveData(a_TargetEntity);
		}

	private:
		std::array<T, MAXENTITIES> m_ComponentList;
		
		std::unordered_map<Entity, uint32_t> m_EntityToIndexMap;
		std::unordered_map<uint32_t, Entity> m_IndexToEntityMap;

		uint32_t m_Size = 0;
	};
}