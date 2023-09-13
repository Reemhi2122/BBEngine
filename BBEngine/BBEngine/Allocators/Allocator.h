#pragma once
#include <cstdint>

#define DEFAULT_ALIGNMENT (2*sizeof(void *))

namespace BBE {
	namespace Allocators{
		
		class Allocator {

		public:
			Allocator() = default;
			~Allocator() = default;

			virtual void Init(const size_t& size) noexcept = 0;
			virtual void* Alloc(const uint32_t& a_Size, const uint32_t& a_Align = DEFAULT_ALIGNMENT) = 0;
			virtual void Realloc(const size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) = 0;
			virtual void Free(void* ptr) = 0;
			virtual void Clear() = 0;

		protected:
			uintptr_t CalculateAlignOffset(uintptr_t a_Ptr, uint32_t a_Align);
		};
	}

	inline bool IsPowerOfTwo(const size_t& x) {
		return (x & (x - 1)) == 0;
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