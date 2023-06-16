#pragma once
#include <set>
#include "../ECSInclude.h"

namespace ECS {
	class System {
	public:
		std::set<Entity> m_Entities;
	};
}