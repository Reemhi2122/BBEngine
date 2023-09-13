#include "ArenaAlloc.h"
#include <malloc.h>
#include <cassert>
#include <string.h>

namespace BBE {
	namespace Allocators {

		void ArenaAllocator::Init(const size_t& a_Size) noexcept
		{
			m_Arena.allocBuf = malloc(a_Size);
			m_Arena.bufLeng = a_Size;
			m_Arena.currOffset = 0u;
			m_Arena.prevOffset = 0u;
		}

		void* ArenaAllocator::Alloc(const uint32_t& a_Size, const uint32_t& a_Align)
		{
			assert(IsPowerOfTwo(a_Align));

			uintptr_t curPointer = (uintptr_t)m_Arena.allocBuf + (uintptr_t)m_Arena.currOffset;
			uintptr_t offset = CalculateAlignOffset(curPointer, a_Align);
			offset -= (uintptr_t)m_Arena.allocBuf;

			if (offset + a_Size <= m_Arena.bufLeng) {
				void* ptr = Add(m_Arena.allocBuf, offset);
				m_Arena.prevOffset = offset;
				m_Arena.currOffset = offset + a_Size;

				memset(ptr, 0, a_Size);
				return ptr;
			}

			return NULL;
		}

		void ArenaAllocator::Realloc(const size_t& a_Size, const size_t& a_Align)
		{
			IsPowerOfTwo(a_Align);
		}

		void ArenaAllocator::Free(void* ptr) noexcept
		{
			// You can't free anything seperate
		}

		void ArenaAllocator::Clear() noexcept
		{
			m_Arena.currOffset = 0u;
			m_Arena.prevOffset = 0u;
		}

	}
}