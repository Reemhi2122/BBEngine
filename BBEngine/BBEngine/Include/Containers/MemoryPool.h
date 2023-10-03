#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE {


	template<typename T>
	class MemoryPool {
	public:
		MemoryPool(size_t maxPool);
		~MemoryPool();

		T* Pop();
		void PushFront(T* a_Element);

	private:
		void InitNewElements(size_t a_num = 5);

		T** m_Head;
		Allocators::ArenaAllocator m_Alloc;
	};

	template<typename T>
	MemoryPool<T>::MemoryPool(size_t a_MaxSize) {
		
		BB_StaticAssert(sizeof(T) >= 8, "Memory pool type size is smaller than pointer size!");
		m_Alloc.Init(sizeof(T) * a_MaxSize);
		InitNewElements(a_MaxSize);
	}

	template<typename T>
	MemoryPool<T>::~MemoryPool() {
		m_Alloc.Clear();
	}

	template<typename T>
	void MemoryPool<T>::InitNewElements(size_t a_num) {
		void* m_Start = BBAlloc(m_Alloc, a_num * sizeof(T), T*);
		m_Head = reinterpret_cast<T**>(m_Start);

		for (size_t i = 0; i < a_num - 1; i++) {
			*m_Head = (reinterpret_cast<T*>(m_Head) + 1);
			m_Head = reinterpret_cast<T**>(*m_Head);
		}

		m_Head = reinterpret_cast<T**>(m_Start);
	}

	template<typename T>
	T* MemoryPool<T>::Pop() {

		BB_Assert(m_Head != NULL, "No more items left in the memory pool");

		T* t = reinterpret_cast<T*>(m_Head);
		m_Head = reinterpret_cast<T**>(*m_Head);
		return t;

	}

	template<typename T>
	void MemoryPool<T>::PushFront(T* a_Element) {
		(*reinterpret_cast<T**>(a_Element)) = reinterpret_cast<T*>(m_Head);
		m_Head = reinterpret_cast<T**>(a_Element);
	}
}