#pragma once

#include "ECSInclude.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

namespace ECS {
	class ECSManager {
	public:
		ECSManager(){
			m_EntityManager = new EntityManager();
			m_SystemManager = new SystemManager();
			m_ComponentManager = new ComponentManager();
		}

		~ECSManager() {
			delete m_EntityManager;
			delete m_SystemManager;
			delete m_ComponentManager;
		}

		Entity CreateEntity() {
			return m_EntityManager->CreateEntity();
		}

		void DestroyEntity(Entity a_Entity) {
			m_EntityManager->DeleteEntity(a_Entity);
			m_ComponentManager->EntityDestroyed(a_Entity);
			m_SystemManager->EntityDestroyed(a_Entity);
		}

		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(Entity a_Entity, T a_Component) {
			m_ComponentManager->AddComponent<T>(a_Entity, a_Component);

			auto t_Signature = m_EntityManager->GetSignature(a_Entity);
			t_Signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(a_Entity, t_Signature);

			m_SystemManager->EntitySignatureChanged(a_Entity, t_Signature);
		}

		template<typename T>
		void RemoveComponent(Entity a_Entity) {
			m_ComponentManager->RemoveComponent<T>(a_Entity);

			auto t_Signature = m_EntityManager->GetSignature(a_Entity);
			t_Signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_EntityManager->SetSignature(a_Entity, t_Signature);

			m_SystemManager->EntitySignatureChanged(a_Entity, t_Signature);
		}

		template<typename T>
		bool HasComponent(Entity a_Entity) {
			auto t_Signature = m_EntityManager->GetSignature(a_Entity);
			return t_Signature.test(m_ComponentManager->GetComponentType<T>());
		}

		template<typename T>
		T& GetComponent(Entity a_Entity) {
			return m_ComponentManager->GetComponent<T>(a_Entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentType<T>();
		}

		template<typename T>
		T* RegisterSystem()
		{
			return m_SystemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature a_Signature)
		{
			m_SystemManager->SetSignature<T>(a_Signature);
		}

	private:
		EntityManager* m_EntityManager;
		SystemManager* m_SystemManager;
		ComponentManager* m_ComponentManager;
	};
}