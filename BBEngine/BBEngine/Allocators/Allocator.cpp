#include "Allocator.h"

namespace BBE {
	namespace Allocators {

		uintptr_t Allocator::CalculateAlignOffset(uintptr_t a_Ptr, uint32_t a_Align)
		{
			uintptr_t modulo;

			modulo = a_Ptr & (a_Align - 1);

			if (modulo != 0) {
				a_Ptr += a_Align - modulo;
			}

			return a_Ptr;
		}
	}
}