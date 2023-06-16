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

		/// <summary>
		/// Create an entity.
		/// </summary>
		/// <returns>Returns the entity ID.</returns>
		Entity CreateEntity() {
			return m_EntityManager->CreateEntity();
		}

		/// <summary>
		/// Destroy the entity.
		/// This also removes it from all the systems and removes
		/// the references to components.
		/// </summary>
		/// <param name="a_Entity">The entity you want to destory</param>
		void DestroyEntity(Entity a_Entity) {
			m_EntityManager->DeleteEntity(a_Entity);
			m_ComponentManager->EntityDestroyed(a_Entity);
			m_SystemManager->EntityDestroyed(a_Entity);
		}

		/// <summary>
		/// This registers the given component
		/// </summary>
		/// <typeparam name="T">The component</typeparam>
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		/// <summary>
		/// Add a component to an enity.
		/// This sets / resets the signature.
		/// </summary>
		/// <typeparam name="T">The component type you want to add.</typeparam>
		/// <param name="a_Entity">The entity you want to add the component to.</param>
		/// <param name="a_Component">The component you want to add.</param>
		template<typename T>
		void AddComponent(Entity a_Entity, T a_Component) {
			m_ComponentManager->AddComponent<T>(a_Entity, a_Component);

			auto t_Signature = m_EntityManager->GetSignature(a_Entity);
			t_Signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(a_Entity, t_Signature);

			m_SystemManager->EntitySignatureChanged(a_Entity, t_Signature);
		}

		/// <summary>
		/// Remove the component from an entity.
		/// This sets / resets the signature.
		/// </summary>
		/// <typeparam name="T">The type of component you want to remove</typeparam>
		/// <param name="a_Entity">The enity you want to remove this component from.</param>
		template<typename T>
		void RemoveComponent(Entity a_Entity) {
			m_ComponentManager->RemoveComponent<T>(a_Entity);

			auto t_Signature = m_EntityManager->GetSignature(a_Entity);
			t_Signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_EntityManager->SetSignature(a_Entity, t_Signature);

			m_SystemManager->EntitySignatureChanged(a_Entity, t_Signature);
		}

		/// <summary>
		/// Checks if the entity has this component.
		/// </summary>
		/// <typeparam name="T">The type of component you want to check.</typeparam>
		/// <param name="a_Entity">The entity you want to check for the component</param>
		/// <returns></returns>
		template<typename T>
		bool HasComponent(Entity a_Entity) {
			auto t_Signature = m_EntityManager->GetSignature(a_Entity);
			return t_Signature.test(m_ComponentManager->GetComponentType<T>());
		}

		/// <summary>
		/// Get the component from the entity.
		/// </summary>
		/// <typeparam name="T">The type of component.</typeparam>
		/// <param name="a_Entity">The entity you want to get the component from.</param>
		/// <returns>The component.</returns>
		template<typename T>
		T& GetComponent(Entity a_Entity) {
			return m_ComponentManager->GetComponent<T>(a_Entity);
		}

		/// <summary>
		/// Gets the component type in the ECS (typeid).
		/// </summary>
		/// <typeparam name="T">The component you want to check.</typeparam>
		/// <returns>The component type.</returns>
		template<typename T>
		ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentType<T>();
		}

		/// <summary>
		/// Register a system to the ECS.
		/// </summary>
		/// <typeparam name="T">The system you want to register.</typeparam>
		/// <returns>The system.</returns>
		template<typename T>
		T* RegisterSystem()
		{
			return m_SystemManager->RegisterSystem<T>();
		}

		/// <summary>
		/// Sets / resets the system signature.
		/// </summary>
		/// <typeparam name="T">The sytem you want to change the signature for.</typeparam>
		/// <param name="a_Signature">The new signature.</param>
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