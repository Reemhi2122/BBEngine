#pragma once
#include <unordered_map>
#include <iostream>

#include "ComponentArray.h"
#include "ECSInclude.h"

namespace ECS {
	class ComponentManager
	{
	public:
		template<typename T>
		bool RegisterComponent() 
		{
			const char* t_TypeName = typeid(T).name();
			
			if(m_ComponentTypes.find(t_TypeName) != m_ComponentTypes.end())
				throw std::runtime_error("ECS || Double registered component detected");

			m_ComponentTypes.insert({t_TypeName, m_NextComponentType});
			m_ComponentArrays.insert({t_TypeName, new ComponentArray<T>()});

			m_NextComponentType++;

			return true;
		}

		template<typename T>
		ComponentType GetComponentType() {
			const char* t_TypeName = typeid(T).name();

			return m_ComponentTypes[t_TypeName];
		}

		template<typename T>
		bool AddComponent(Entity a_TargetEntity, T a_Component)
		{
			GetComponentArray<T>()->InsertComponent(a_TargetEntity, a_Component);
			return true;
		}

		template<typename T>
		T& GetComponent(Entity a_TargetEntity)
		{
			return GetComponentArray<T>()->GetComponent(a_TargetEntity);
		}

		template<typename T>
		bool RemoveComponent(Entity a_TargetEntity)
		{
			GetComponentArray<T>()->RemoveData(a_TargetEntity);
			return true;
		}

		void EntityDestroyed(Entity a_TargetEntity)
		{
			for (auto const& pair : m_ComponentArrays)
				pair.second->EntityDestroyed(a_TargetEntity);
		}

	private:
		template<typename T>
		ComponentArray<T>* GetComponentArray()
		{
			const char* t_TypeName = typeid(T).name();
			
			if(m_ComponentTypes.find(t_TypeName) == m_ComponentTypes.end())
				throw std::runtime_error("ECS || Can't find component");
			
			return static_cast<ComponentArray<T>*>(m_ComponentArrays[t_TypeName]);
		}

		std::unordered_map<const char*, ComponentType> m_ComponentTypes{};
		std::unordered_map<const char*, IComponentArray*> m_ComponentArrays{};

		ComponentType m_NextComponentType{};
	};
}