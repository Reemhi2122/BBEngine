#pragma once
#include "Allocator.h"

namespace BBE {
	namespace Allocators {

		struct PoolNode {
			PoolNode* next;
		};

		struct Pool {
			void* buf;
			size_t bufLength;
			size_t chunkSize;
			PoolNode* head;
		};

		class PoolAllocator : public Allocator
		{
		public:
			PoolAllocator();
			~PoolAllocator();
			
			PoolAllocator& operator=(const PoolAllocator& rhs) = delete;

			void Init(const size_t& a_Size, const size_t a_Allignment = DEFAULT_ALIGNMENT, const size_t& a_ChunkSize = 0) override;
			void* Alloc(size_t& a_Size = 0, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override; //There is no realloc because size is constant
			void Free(void* a_Ptr) override;
			void Clear() override;

		private:
			Pool m_Pool;
		};

	}
}