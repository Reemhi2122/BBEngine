#include "Allocator.h"

namespace BBE {
	namespace Allocators {

		size_t Allocator::CalculateAlignOffset(uintptr_t a_Ptr, uint32_t a_Align, const size_t& a_HeaderSize)
		{
			uintptr_t modulo = 0;
			uintptr_t padding = 0;
			uintptr_t neededSpace = 0;

			modulo = a_Ptr & (a_Align - 1);

			if (modulo != 0) {
				padding = a_Align - modulo;
			}

			if(a_HeaderSize <= 0)
				return padding; // 12
		
			neededSpace = a_HeaderSize; // 16

			if (padding < neededSpace) {
				neededSpace -= padding; // 4

				if ((neededSpace & (a_Align - 1)) != 0) {
					padding += a_Align * (1 + (neededSpace / a_Align));
				}
				else {
					padding += a_Align * (neededSpace / a_Align);
				}
			}

			return padding;
		}
	}
}