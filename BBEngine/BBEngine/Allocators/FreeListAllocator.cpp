#include "FreeListAllocator.h"
#include <malloc.h>
#include "../Logger/Logger.h"

namespace BBE {
	namespace Allocators {

		FreeListAllocator::FreeListAllocator()
		{
			m_FreeList.buffer = NULL;
			m_FreeList.head = NULL;
			m_FreeList.policy = PlacementPolicy::FindFirst;
			m_FreeList.size = 0u;
			m_FreeList.used = 0u;
		}

		FreeListAllocator::~FreeListAllocator()
		{
			m_FreeList.buffer = NULL;
			m_FreeList.head = NULL;
		}

		void FreeListAllocator::Init(const size_t& a_Size, const size_t& a_ChunkSize)
		{
			m_FreeList.buffer = malloc(a_Size);
			m_FreeList.size = a_Size;

			Clear();
		}

		void* FreeListAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
		{
			size_t padding;
			FreeListNode* prevNode;
			FreeListNode* curNode;
			
			BB_Assert(a_Size > sizeof(FreeListNode), "Size is less than required for node");
			BB_Assert(a_Align > 8, "Align is too small");
	
			if (m_FreeList.policy == PlacementPolicy::FindFirst) {
				curNode = FindFirstNode(a_Size, a_Align, padding, prevNode);
			}
			else {
				curNode = FindBestNode(a_Size, a_Align, padding, prevNode);
			}

			BB_Assert(curNode != NULL, "Freelist is out of memory!");

			size_t headerPadding = padding - sizeof(FreeListHeader);
			size_t requiredSpace = padding + a_Size;
			size_t remaining = curNode->blockSize - requiredSpace;
			
			if (remaining > 0) {
				FreeListNode* newNode = (FreeListNode*)((char*)curNode + requiredSpace);
				newNode->blockSize = remaining;
				InsertNode(m_FreeList.head, curNode, newNode);
			}

			RemoveNode(m_FreeList.head, prevNode, curNode);

			FreeListHeader* header = reinterpret_cast<FreeListHeader*>((char*)curNode + headerPadding);
			header->blockSize = requiredSpace;
			header->padding = headerPadding;
			
			m_FreeList.used = requiredSpace;

			return Pointer::Add(reinterpret_cast<void*>(header), sizeof(FreeListHeader));
		}

		void* FreeListAllocator::Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
		{
			return NULL;
		}

		void FreeListAllocator::Free(void* a_Ptr)
		{
			if (a_Ptr == NULL)
				return;
		
			FreeListHeader* header = reinterpret_cast<FreeListHeader*>(Pointer::Add(a_Ptr, sizeof(FreeListHeader)));
			FreeListNode* newNode = reinterpret_cast<FreeListNode*>(header);
			FreeListNode* node = m_FreeList.head;
			FreeListNode* prevNode = NULL;

			newNode->blockSize = header->blockSize + header->padding;
			newNode->next = NULL;

			while (node != NULL)
			{
				if (reinterpret_cast<uintptr_t>(a_Ptr) < reinterpret_cast<uintptr_t>(node)) {
					InsertNode(m_FreeList.head, prevNode, newNode);
					break;
				}
				prevNode = node;
				node = node->next;
			}

			m_FreeList.used -= newNode->blockSize;

			FreeCoalescence(prevNode, newNode);
		}

		void FreeListAllocator::FreeCoalescence(FreeListNode* a_PrevNode, FreeListNode* a_FreeNode)
		{
			if (a_FreeNode->next != NULL && a_FreeNode + a_FreeNode->blockSize == a_FreeNode->next) {
				a_FreeNode->blockSize += a_FreeNode->next->blockSize;
				RemoveNode(m_FreeList.head, a_FreeNode, a_FreeNode->next);
			}

			if (a_PrevNode->next != NULL && a_PrevNode + a_PrevNode->blockSize == a_FreeNode) {
				a_PrevNode->blockSize += a_FreeNode->blockSize;
				RemoveNode(m_FreeList.head, a_PrevNode, a_PrevNode->next);
			}
		}

		void FreeListAllocator::Clear()
		{
			m_FreeList.used = 0;
			FreeListNode* firstNode = reinterpret_cast<FreeListNode*>(m_FreeList.buffer);
			firstNode->blockSize = m_FreeList.size;
			firstNode->next = NULL;
			m_FreeList.head = firstNode;
		}

		FreeListNode* FreeListAllocator::FindFirstNode(const size_t& a_Size, const size_t& a_Align, size_t& a_Padding, FreeListNode*& a_PrevNode)
		{
			FreeListNode* node = m_FreeList.head;
			FreeListNode* prevNode = NULL;

			size_t padding = 0;

			while (node != NULL) {
				padding = CalculateAlignOffset((uintptr_t)node, a_Align, sizeof(FreeListHeader));

				size_t requiredSize = a_Size + padding;
				if (node->blockSize >= requiredSize) {
					break;
				}

				prevNode = node;
				node = node->next;
			}

			a_Padding = padding;
			a_PrevNode = prevNode;
			return node;
		}

		FreeListNode* FreeListAllocator::FindBestNode(const size_t& a_Size, const size_t& a_Alignment, size_t& a_Padding, FreeListNode*& a_PrevNode)
		{
			FreeListNode* node = m_FreeList.head;
			FreeListNode* prevNode = NULL;
			FreeListNode* bestNode = NULL;

			size_t padding = 0;

			while (node != NULL) {
				padding = CalculateAlignOffset((uintptr_t)node, a_Alignment, sizeof(FreeListHeader));
				size_t requiredSize = padding + a_Size;

				if (node->blockSize >= requiredSize) {
					if (bestNode == NULL || node->blockSize < bestNode->blockSize) {
						bestNode = node;
					}
				}

				prevNode = node;
				node = node->next;
			}

			a_Padding = padding;
			a_PrevNode = prevNode;
			return bestNode;
		}

		void FreeListAllocator::InsertNode(FreeListNode*& a_Head, FreeListNode* a_PrevNode, FreeListNode* a_NewNode)
		{
			if (a_PrevNode == NULL) {
				if (a_Head != NULL) {
					a_NewNode->next = a_Head; // Only thing I am quite unsure of?
				}
				else {
					a_Head = a_NewNode;
				}
			}
			else {
				if (a_PrevNode->next == NULL) {
					a_PrevNode->next = a_NewNode;
					a_NewNode->next = NULL;
				}
				else {
					a_NewNode->next = a_PrevNode->next;
					a_PrevNode->next = a_NewNode;
				}
			}
		}

		void FreeListAllocator::RemoveNode(FreeListNode*& a_Head, FreeListNode* a_PrevNode, FreeListNode* a_DelNode)
		{
			if (a_PrevNode == NULL) {
				a_Head = a_DelNode->next;
			}
			else {
				a_PrevNode->next = a_DelNode->next;
			}
		}
	}
}