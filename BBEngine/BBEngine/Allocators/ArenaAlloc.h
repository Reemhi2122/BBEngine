#pragma once
#include "Allocator.h"
#include <cstdint>

#define DEFAULT_ALIGNMENT (2*sizeof(void *))

namespace BBE {
	namespace Allocators {

		struct Arena {
			void* allocBuf;
			unsigned int bufLeng;
			unsigned int prevOffset;
			unsigned int currOffset; 
		};

		class ArenaAllocator : public Allocator {
			
		public:
			ArenaAllocator() = default;
			~ArenaAllocator() = default;

			void Init(const uint32_t& a_Size) noexcept;

			void* Alloc(const uint32_t& a_Size, const uint32_t& a_Align = DEFAULT_ALIGNMENT);
			void Realloc() ;
			void Free(void* ptr) noexcept;
			void FreeAll() noexcept;

		private:
			Arena m_Arena;

		};

	}
}