#pragma once
#include "allocator.h"
#include <malloc.h>
#include <cassert>
#include "../Logger/Logger.h"

namespace BBE {
	namespace Allocators {

		struct Stack {
			void* buf;
			size_t bufSize;
			size_t offset;
		};

		struct StackHeader {
			uint8_t padding;
		};

		class StackAllocator : public Allocator {
			
		public:
			StackAllocator();
			~StackAllocator() = default;

			void Init(const size_t& a_Size) noexcept;
			void* Alloc(const size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT);
			void* Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT);
			void Free(void* a_Ptr);
			void Clear();

		private:
			Stack m_Stack;

		};

	}
}