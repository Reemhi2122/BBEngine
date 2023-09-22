#include "Allocator.h"

#include <Windows.h>
#include <memoryapi.h>

namespace BBE {
	namespace Allocators {

		size_t Allocator::CalculateAlignOffset(const uintptr_t& a_Ptr, const uint32_t& a_Align, const size_t& a_HeaderSize)
		{
			uintptr_t modulo = 0;
			uintptr_t padding = 0;
			uintptr_t neededSpace = 0;

			modulo = a_Ptr & (a_Align - 1);

			if (modulo != 0) {
				padding = a_Align - modulo;
			}

			if(a_HeaderSize <= 0)
				return padding;
		
			neededSpace = a_HeaderSize;

			if (padding < neededSpace) {
				neededSpace -= padding;

				if ((neededSpace & (a_Align - 1)) != 0) {
					padding += a_Align * (1 + (neededSpace / a_Align));
				}
				else {
					padding += a_Align * (neededSpace / a_Align);
				}
			}

			return padding;
		}

		uintptr_t Allocator::AlignForward(uintptr_t ptr, uintptr_t align) {
			uintptr_t a, p, modulo;

			a = align;
			p = ptr;
			modulo = p & (a - 1);
			if (modulo != 0) {
				p += a - modulo;
			}
			return p;
		}

		void* Allocator::AllocVirtual(size_t& a_Size)
		{
			constexpr int pageSize = 4096;
			constexpr int virtualExpansion = 64;

			a_Size += pageSize - (a_Size % pageSize);
			void* ptr = VirtualAlloc(NULL, a_Size * virtualExpansion, MEM_RESERVE, PAGE_NOACCESS);

			return VirtualAlloc(ptr, a_Size, MEM_COMMIT, PAGE_READWRITE);
		}

		void Allocator::ResizeVirtual(void* a_Ptr, size_t& a_Size)
		{
			constexpr int resizeMultiply = 2;
			a_Size *= resizeMultiply;
			VirtualAlloc(a_Ptr, a_Size, MEM_COMMIT, PAGE_READWRITE);
		}

		void Allocator::FreeVirtual(void* a_Ptr)
		{
			VirtualFree(a_Ptr, 0, MEM_RELEASE);
		}
	}
}
