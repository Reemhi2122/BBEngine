#include "PoolAllocator.h"
#include <malloc.h>
#include <cassert>
#include "../Logger/Logger.h"

namespace BBE {
	namespace Allocators {

		PoolAllocator::PoolAllocator()
		{
			m_Pool.buf = NULL;
			m_Pool.bufLength = 0u;
			m_Pool.chunkSize = 0u;
			m_Pool.head = NULL;
		}

		PoolAllocator::~PoolAllocator()
		{

		}

		void PoolAllocator::Init(const size_t& a_Size, const size_t& a_ChunkSize)
		{
			m_Pool.buf = malloc(a_Size);

			uintptr_t initialStart = (uintptr_t)m_Pool.buf;
			uintptr_t start = AlignForward(initialStart, DEFAULT_ALIGNMENT);
			size_t bufLength = a_Size - (start - initialStart);
			
			size_t chunkSize = AlignForward((uintptr_t)a_ChunkSize, DEFAULT_ALIGNMENT);

			BB_Assert((chunkSize >= sizeof(PoolNode)), "Chunk size too small for header");
			BB_Assert((bufLength >= chunkSize), "Backing buffer length is small than te chunk size");

			m_Pool.bufLength = bufLength;
			m_Pool.chunkSize = chunkSize;
			m_Pool.head = NULL;

			Clear();
		}

		void* PoolAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
		{
			PoolNode* node = m_Pool.head;
			
			if (node == NULL) {
				BB_Assert(0, "Pool allocator has no free memory");
				return NULL;
			}
			
			m_Pool.head = node->next;
			
			return memset(node, 0, m_Pool.chunkSize);
		}

		void* PoolAllocator::Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
		{
			return NULL;
		}

		void PoolAllocator::Free(void* a_Ptr)
		{
			PoolNode* node;
			void* end = Add(m_Pool.buf, m_Pool.bufLength);

			if (a_Ptr == NULL) {
				return;
			}

			BB_Assert(!(m_Pool.buf <= a_Ptr && a_Ptr < end), "Memory is out of bounds of pool allocator");

			memset(a_Ptr, 0, m_Pool.chunkSize);

			node = reinterpret_cast<PoolNode*>(a_Ptr);
			node->next = m_Pool.head;
			m_Pool.head = node;
		}

		void PoolAllocator::Clear()
		{
			size_t chunkCount = m_Pool.bufLength / m_Pool.chunkSize;
			
			for (size_t i = 0; i < chunkCount; i++) {
				void* ptr = Add(m_Pool.buf, (i + m_Pool.chunkSize));
				PoolNode* node = reinterpret_cast<PoolNode*>(ptr);
				node->next = m_Pool.head;
				m_Pool.head = node;
			}
		}

	}
}