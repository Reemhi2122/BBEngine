#pragma once
#include <cstdint>

namespace ECS {
	struct Mesh {
		uint32_t m_MeshHandle = 0;
		uint32_t m_MaterialHandle = 0;
		bool m_Render = true;
	};
}