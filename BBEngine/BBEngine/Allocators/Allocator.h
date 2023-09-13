#pragma once
#include <cstdint>

namespace BBE {
	namespace Allocators{
		
		class Allocator {

		public:
			Allocator() = default;
			~Allocator() = default;

		protected:
			uintptr_t CalculateAlignOffset(uintptr_t a_Ptr, uint32_t a_Align);
		};
	}

	inline static void* Add(const void* a_Ptr, const size_t a_Add)
	{
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(a_Ptr) + a_Add);
	}

	inline static void* Subtract(const void* a_Ptr, const size_t a_Subtract)
	{
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(a_Ptr) - a_Subtract);
	}
}