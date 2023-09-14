#pragma once
#include "allocator.h"

namespace BBE {
	namespace Allocators {

		struct Stack {
			void* buf;
		};

		class StackAllocator : public Allocator {
		


		public:
			StackAllocator();
			~StackAllocator();

		private:
			void* Alloc(const size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT);
			void* Realloc(void* a_OldData, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT);
			void Free(void* ptr);
			void Clear();
		};

	}
}