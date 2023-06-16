#pragma once
#include <unordered_map>

#include "systems/System.h"
#include "ECSInclude.h"

namespace ECS {
	class SystemManager {
	public:
		template<typename T>
		T* RegisterSystem() {
			const char* t_TypeName = typeid(T).name();

			if (m_Systems.find(t_TypeName) != m_Systems.end())
				throw std::runtime_error("ECS || Double registered system detected");

			auto t_System = new T();
			m_Systems.insert({t_TypeName, t_System});
			return t_System;
		}

		template<typename T>
		void SetSignature(Signature a_Signature) {
			const char* t_TypeName = typeid(T).name();
			m_Signatures.insert({t_TypeName, a_Signature});
		}

		void EntityDestroyed(Entity a_Entity) {
			for (auto const& pair : m_Systems) {
				auto const& t_System = pair.second;
				t_System->m_Entities.erase(a_Entity);
			}
		}

		void EntitySignatureChanged(Entity a_Entity, Signature a_EntitySignature) {
			for (auto const& pair : m_Systems) {
				auto const& t_Type = pair.first;
				auto const& t_System = pair.second;
				auto const& t_SystemSignature = m_Signatures[t_Type];

				if ((a_EntitySignature & t_SystemSignature) == t_SystemSignature) {
					t_System->m_Entities.insert(a_Entity);
				} else {
					t_System->m_Entities.erase(a_Entity);
				}
			}
		}

	private:
		std::unordered_map<const char*, Signature> m_Signatures{};
		std::unordered_map<const char*, System*> m_Systems{};
	};
}