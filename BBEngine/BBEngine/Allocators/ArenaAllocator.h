#pragma once
#include "Allocator.h"

namespace BBE {
	namespace Allocators {

		struct Arena {
			void* buf;
			size_t bufLeng;
			size_t prevOffset;
			size_t currOffset;
		};

		class ArenaAllocator : public Allocator {
			
		public:
			ArenaAllocator();
			~ArenaAllocator();
			
			ArenaAllocator& operator=(const ArenaAllocator& rhs) = delete;

			void Init(const size_t& a_Size, const size_t a_Allignment = DEFAULT_ALIGNMENT, const size_t& a_ChunkSize = 0) override;
			void* Alloc(size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void Free(void* a_Ptr) noexcept override;
			void Clear() noexcept override;

		private:
			Arena m_Arena;

		};

	}
}