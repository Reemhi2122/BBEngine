#pragma once
#include "Allocator.h"

namespace BBE {
	namespace Allocators {

		struct FreeListHeader {
			size_t padding;
			size_t blockSize;
		};

		struct FreeListNode {
			FreeListNode* next;
			size_t blockSize;
		};

		enum class PlacementPolicy {
			FindFirst,
			FindBest
		};

		struct FreeList {
			void* buffer;
			size_t size;
			size_t used;

			FreeListNode* head;
			PlacementPolicy policy;
		};

		class FreeListAllocator : public Allocator
		{
		public:
			FreeListAllocator();
			~FreeListAllocator();

			void Init(const size_t& a_Size, const size_t& a_ChunkSize = 0) override;
			void* Alloc(const size_t& a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void Free(void* a_Ptr) override;
			void Clear() override;

		private:
			FreeListNode* FindFirstNode(const size_t& a_Size, const size_t& a_Align, size_t& a_Padding, FreeListNode*& a_PrevNode);
			FreeListNode* FindBestNode(const size_t& a_Size, const size_t& a_Alignment, size_t& a_Padding, FreeListNode*& a_PrevNode);
	
			void InsertNode(FreeListNode*& a_Head, FreeListNode* a_PrevNode, FreeListNode* a_NewNode);
			void RemoveNode(FreeListNode*& a_Head, FreeListNode* a_PrevNode, FreeListNode* a_DelNode);

			void FreeCoalescence(FreeListNode* a_PrevNode, FreeListNode* a_FreeNode);

			FreeList m_FreeList;

		};

	}
}