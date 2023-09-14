#pragma once
#include "Allocator.h"

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

			void Init(const size_t& a_Size) noexcept override;
			void* Alloc(const uint32_t& a_Size, const uint32_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_OldData, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void Free(void* ptr) noexcept override;
			void Clear() noexcept override;

		private:
			Arena m_Arena;

		};

	}
}