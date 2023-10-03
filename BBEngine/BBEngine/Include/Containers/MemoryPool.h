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

		size_t m_Size;
		size_t m_MaxSize;
	};

	template<typename T>
	MemoryPool<T>::MemoryPool(size_t a_MaxSize) {
		m_MaxSize = a_MaxSize;
		m_Alloc.Init(sizeof(T) * m_MaxSize);
		InitNewElements();
	}

	template<typename T>
	MemoryPool<T>::~MemoryPool() {
		m_Alloc.Clear();
	}

	template<typename T>
	void MemoryPool<T>::InitNewElements(size_t a_num) {
		m_Head = &BBNewArr(m_Alloc, a_num, T);

		for (size_t i = 0; i < a_num; i++) {
			*m_Head = (reinterpret_cast<T*>(*m_Head)) + 1;
			m_Head = reinterpret_cast<T**>(*m_Head);
			m_Size++;
		}
	}

	template<typename T>
	T* MemoryPool<T>::Pop() {

		BB_Assert(m_Head == NULL, "No more items left in the memory pool");

		T* t = reinterpret_cast<T*>(m_Head);
		m_Head = reinterpret_cast<T**>(*m_Head);
		return t;

	}

	template<typename T>
	void MemoryPool<T>::PushFront(T* a_Element) {
		PoolElement<T> el;
		el.element = a_Element;
		el.nextElement = m_Head;
		m_Head = &el;
	}
}