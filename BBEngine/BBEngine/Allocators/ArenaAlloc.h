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
			~ArenaAllocator() = default;

			void Init(const size_t& a_Size) noexcept override;
			void* Alloc(const size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_OldData, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void Free(void* ptr) noexcept override;
			void Clear() noexcept override;

		private:
			Arena m_Arena;

		};

	}
}