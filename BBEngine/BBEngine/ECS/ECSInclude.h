#pragma once
#include <cstdint>
#include <bitset>
#include <stdexcept>

namespace ECS {
	const uint32_t MAXENTITIES = 5000;
	using Entity = uint32_t;

	using ComponentType = std::uint8_t;

	const uint32_t MAX_COMPONENTS = 32;
	using Signature = std::bitset<MAX_COMPONENTS>;
}