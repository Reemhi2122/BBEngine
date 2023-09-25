#pragma once
#include <cstdint>

#define DEFAULT_ALIGNMENT (2*sizeof(void *))

namespace BBE {
	namespace Allocators{
		
		class Allocator {

		public:
			Allocator() = default;
			~Allocator() = default;

			virtual void Init(size_t a_Size, const size_t& a_Allignment = DEFAULT_ALIGNMENT, const size_t& a_ChunkSize = 0) = 0;
			virtual void* Alloc(size_t a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) = 0;
			virtual void* Realloc(void* a_Ptr, size_t a_OldSize, size_t a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) = 0;
			virtual void Free(void* a_Ptr) = 0;
			virtual void Clear() = 0;

		protected:
			size_t CalculateAlignOffset(const uintptr_t& a_Ptr, const uint32_t& a_Align, const size_t& a_HeaderSize = 0);
			uintptr_t AlignForward(uintptr_t ptr, uintptr_t align);
	
			void* AllocVirtual(size_t a_Size);
			void ResizeVirtual(void* a_Ptr, size_t& a_Size);
			void FreeVirtual(void* a_Ptr);

			size_t m_VirtualSize;
		};
	}

	inline bool IsPowerOfTwo(const size_t& x) {
		return (x & (x - 1)) == 0;
	}

	namespace Pointer {
		inline static void* Add(const void* a_Ptr, const size_t a_Add)
		{
			return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(a_Ptr) + a_Add);
		}

		inline static void* Add(uintptr_t& a_Ptr, const size_t a_Add)
		{
			return reinterpret_cast<void*>(a_Ptr + a_Add);
		}

		inline static void* Subtract(const void* a_Ptr, const size_t a_Subtract)
		{
			return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(a_Ptr) - a_Subtract);
		}
	}
}