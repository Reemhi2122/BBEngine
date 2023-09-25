#pragma once
#include "Allocators/Allocator.h"
#include <type_traits>

template <typename T>
struct MacroType { typedef T type; }; //I hate C++.

#define BBAlloc(a_Allocator, a_Size, a_Type) BBE::BBAllocFunc(a_Allocator, a_Size)
#define BBNew(a_Allocator, a_Type) new (BBE::BBAllocFunc(a_Allocator, sizeof(a_Type))) a_Type
#define BBNewArr(a_Allocator, a_Num, a_Type) BBE::BBAllocArrayFunc<a_Type>(a_Allocator, sizeof(a_Type), a_Num)

#define BBFree(a_Allocator, a_Pointer) BBE::BBFreeFunc(a_Allocator, a_Pointer);

namespace BBE {

	struct BBArrayAllocHeader
	{
		size_t arraySize;
	};

	inline void* BBAllocFunc(Allocators::Allocator& a_Allocator, const size_t& a_Size) {
		return a_Allocator.Alloc(a_Size);
	}

	template<typename T>
	inline T* BBAllocArrayFunc(Allocators::Allocator& a_Allocator, const size_t& a_Size, const size_t a_Count) {
		
		if constexpr (std::is_trivially_constructible<T>() && std::is_trivially_destructible<T>()) {
			return reinterpret_cast<T>(BBAllocFunc(a_Allocator, a_Size));
		}
		else {
			size_t headerSize = sizeof(BBArrayAllocHeader);
			
			T* ptr = reinterpret_cast<T*>(a_Allocator.Alloc(a_Size + headerSize));

			if (!std::is_trivially_constructible<T>()) {
				for (size_t i = 0; i < a_Count; i++)
				{
					new (&ptr[i])T();
				}	
			}

			return ptr;
		}

		return nullptr;
	}

	inline void BBFreeFunc(Allocators::Allocator& a_Allocator, void* a_Ptr) {
		a_Allocator.Free(a_Ptr);
	}

}