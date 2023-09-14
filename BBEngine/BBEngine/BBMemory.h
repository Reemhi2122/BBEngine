#pragma once
#include "Allocators/Allocator.h"

#define BBAlloc(a_Allocator, a_Size, a_Type) BBE::BBAllocFunc(a_Allocator, a_Size)
#define BBNew(a_Allocator, a_Type) new (BBE::BBAllocFunc(a_Allocator, sizeof(a_Type))) a_Type
#define BBNewArr(a_Allocator, a_Num, a_Type) BBE::BBAllocFunc(a_Allocator, sizeof(a_Type) * a_Num)

#define BBFree()

namespace BBE {

	inline void* BBAllocFunc(Allocators::Allocator& a_Allocator, const size_t& a_Size) {
		return a_Allocator.Alloc(a_Size);
	}

	inline void BBFreeFunc(Allocators::Allocator& a_Allocator, void* a_Ptr) {
		a_Allocator.Free(a_Ptr);
	}

}