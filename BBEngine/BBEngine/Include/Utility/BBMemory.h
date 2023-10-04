#pragma once
#include "Allocators/Allocator.h"
#include "Allocators/ArenaAllocator.h"
#include "Allocators/StackAllocator.h"
#include "Allocators/PoolAllocator.h"
#include "Allocators/FreeListAllocator.h"
#include "Allocators/BuddyAllocator.h"
#include <type_traits>

#define BBAlloc(a_Allocator, a_Size, a_Type) reinterpret_cast<a_Type>(BBE::BBAllocFunc(a_Allocator, a_Size))
#define BBNew(a_Allocator, a_Type) new (BBE::BBAllocFunc(a_Allocator, sizeof(a_Type))) a_Type
#define BBNewArr(a_Allocator, a_Num, a_Type) BBE::BBAllocArrayFunc<a_Type>(a_Allocator, sizeof(a_Type), a_Num)

#define BBFree(a_Allocator, a_Pointer) BBE::BBFreeFunc(a_Allocator, a_Pointer);
#define BBFreeArr(a_Allocator, a_Pointer) BBE::BBFreeArrFunc(a_Allocator, a_Pointer);

#define BBStackScope(a_Allocator) for (int i = a_Allocator.SetPoint(); i; a_Allocator.ReturnToPoint(), i = 0)

namespace BBE {

	constexpr unsigned int PageSize = 4096;

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
			size_t allocationSize = a_Size * a_Count;
			
			T* ptr = reinterpret_cast<T*>(a_Allocator.Alloc(allocationSize + headerSize));

			BBArrayAllocHeader* header = reinterpret_cast<BBArrayAllocHeader*>(ptr);
			header->arraySize = a_Count;

			Pointer::Add(ptr, headerSize);

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

	template<typename T>
	inline void BBFreeFunc(Allocators::Allocator& a_Allocator, T* a_Ptr) {
		
		if constexpr (!std::is_trivially_destructible<T>()) {
			a_Ptr->~T();
		}
		a_Allocator.Free(a_Ptr);
	}

	template<typename T>
	inline void BBFreeArrFunc(Allocators::Allocator& a_Allocator, T* a_Ptr) {
		if constexpr (std::is_trivially_constructible<T>() && std::is_trivially_destructible<T>()) {
			a_Allocator.Free(a_Ptr);
		}
		else {
			size_t headerSize = sizeof(BBArrayAllocHeader);
			BBArrayAllocHeader* header = reinterpret_cast<BBArrayAllocHeader*>(Pointer::Subtract(a_Ptr, headerSize));

			for (size_t i = 0; i < header->arraySize; i++) {
				a_Ptr[i].~T();
			}

			a_Allocator.Free(Pointer::Subtract(a_Ptr, headerSize));
		}
	}

}