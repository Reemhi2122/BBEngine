#pragma once
#include "Allocator.h"

namespace BBE {
	namespace Allocators {

		class BuddyAllocator : public Allocator
		{
		public:
			BuddyAllocator();
			~BuddyAllocator();

			void Init(const size_t& a_Size, const size_t& a_ChunkSize = 0) override;
			void* Alloc(const size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void Free(void* a_Ptr) override;
			void Clear() override;

		private:

		};
	
	}
}