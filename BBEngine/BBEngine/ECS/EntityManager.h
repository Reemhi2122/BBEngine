#pragma once
#include "CircularEntityQueue.h"
#include <array>
#include <cassert>

namespace ECS {
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		Entity CreateEntity();
		bool DeleteEntity(Entity a_TargetEntity);

		Signature GetSignature(Entity a_TargetEntity);
		void SetSignature(Entity entity, Signature signature);

	private:
		//std::queue<Entity> m_AvailableEntities{};
		CircularEntityQueue m_AvailableEntities;
		std::array<Signature, MAXENTITIES> m_Signatures{};
		uint32_t m_ExistingEntities;
	};
}