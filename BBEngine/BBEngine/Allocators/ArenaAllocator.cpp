#include "ArenaAllocator.h"
#include <memoryapi.h>
#include "../Logger/Logger.h"

namespace BBE {
	namespace Allocators {
		ArenaAllocator::ArenaAllocator()
		{
			m_Arena.buf = nullptr;
			m_Arena.bufLeng = 0u;
			m_Arena.currOffset = 0u;
			m_Arena.prevOffset = 0u;
		}

		void ArenaAllocator::Init(const size_t& a_Size, const size_t a_Allignment, const size_t& a_ChunkSize)
		{
			size_t size = a_Size;

			m_Arena.buf = AllocVirtual(size);
			m_Arena.bufLeng = size;
			m_Arena.currOffset = 0u;
			m_Arena.prevOffset = 0u;
			
		}

		void* ArenaAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
		{
			BB_Assert(IsPowerOfTwo(a_Align), "Align is not a power of two");

			uintptr_t curPointer = (uintptr_t)m_Arena.buf + (uintptr_t)m_Arena.currOffset;
			uintptr_t offset = curPointer + CalculateAlignOffset(curPointer, a_Align);
			offset -= (uintptr_t)m_Arena.buf;

			if (offset + a_Size <= m_Arena.bufLeng) {
				void* ptr = Pointer::Add(m_Arena.buf, offset);
				m_Arena.prevOffset = offset;
				m_Arena.currOffset = offset + a_Size;

				memset(ptr, 0, a_Size);
				return ptr;
			}

			BB_Assert(0, "Allocator is out of memory!");
			return NULL;
		}

		//Reallocation for linear allocators are cringe
		void* ArenaAllocator::Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
		{
			BB_Assert(IsPowerOfTwo(a_Align), "Align is not a power of two");

			uintptr_t& curBuf = reinterpret_cast<uintptr_t&>(m_Arena.buf);
			uintptr_t& oldData = reinterpret_cast<uintptr_t&>(a_Ptr);

			if (a_OldSize == 0 || oldData == NULL) {
				return Alloc(a_NewSize, a_Align);
			}

			if (oldData >= curBuf && oldData < (curBuf + m_Arena.bufLeng)) {
				if (curBuf + m_Arena.prevOffset == oldData) {
					
					if (a_NewSize > a_OldSize) {
						memset(Pointer::Add(curBuf, m_Arena.prevOffset + a_OldSize), 0, a_NewSize - a_OldSize);
					}

					m_Arena.currOffset = m_Arena.prevOffset + a_NewSize;
					return a_Ptr;
				}
			}
			else {
				void* newMemory = Alloc(a_NewSize);
				size_t copySize = a_NewSize > a_OldSize ? a_OldSize : a_NewSize;
				memmove(newMemory, a_Ptr, copySize);
				return newMemory;
			}

			BB_Assert(0, "Memory is out of bounds of the buffer of the allocator");
			return NULL;
		}

		void ArenaAllocator::Free(void* a_Ptr) noexcept
		{
			// There is no free function on a linear allocator
		}

		void ArenaAllocator::Clear() noexcept
		{
			m_Arena.currOffset = 0u;
			m_Arena.prevOffset = 0u;
		}

	}
}