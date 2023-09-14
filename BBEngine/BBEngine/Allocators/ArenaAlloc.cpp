#include "ArenaAlloc.h"
#include <malloc.h>
#include <cassert>
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

		void ArenaAllocator::Init(const size_t& a_Size) noexcept
		{
			m_Arena.buf = malloc(a_Size);
			m_Arena.bufLeng = a_Size;
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
				void* ptr = Add(m_Arena.buf, offset);
				m_Arena.prevOffset = offset;
				m_Arena.currOffset = offset + a_Size;

				memset(ptr, 0, a_Size);
				return ptr;
			}

			BB_Assert(0, "Allocator is out of memory!");
			return NULL;
		}

		//Reallocation for linear allocators are cringe
		void* ArenaAllocator::Realloc(void* a_OldData, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
		{
			BB_Assert(IsPowerOfTwo(a_Align), "Align is not a power of two");

			uintptr_t& CurBuf = reinterpret_cast<uintptr_t&>(m_Arena.buf);
			uintptr_t& OldData = reinterpret_cast<uintptr_t&>(a_OldData);

			if (a_OldSize == 0 || a_OldData == NULL) {
				return Alloc(a_NewSize, a_Align);
			}

			if (OldData >= CurBuf && OldData < (CurBuf + m_Arena.bufLeng)) {
				if (CurBuf + m_Arena.prevOffset == OldData) {
					
					if (a_NewSize > a_OldSize) {
						memset(Add(CurBuf, m_Arena.prevOffset + a_OldSize), 0, a_NewSize - a_OldSize);
					}

					m_Arena.currOffset = m_Arena.prevOffset + a_NewSize;
					return a_OldData;
				}
			}
			else {
				void* newMemory = Alloc(a_NewSize);
				size_t copySize = a_NewSize > a_OldSize ? a_OldSize : a_NewSize;
				memmove(newMemory, a_OldData, copySize);
				return newMemory;
			}

			BB_Assert(0, "Memory is out of bounds of the buffer of the allocator");
			return NULL;
		}

		void ArenaAllocator::Free(void* ptr) noexcept
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