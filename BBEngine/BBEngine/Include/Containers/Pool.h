#pragma once
#include "Utility/BBMemory.h"

namespace BBE {

	template<typename T>
	struct PoolElement {
		T* element;
		PoolElement<T>* nextElement;
	};

	template<typename T>
	class Pool {
	public:
		Pool(size_t maxPool);
		~Pool();

		T* Pop();
		void PushFront(T* a_Element);

	private:
		void InitNewElements(size_t a_num = 5);

		PoolElement<T>* m_Head;
		Allocators::ArenaAllocator m_Alloc;
		size_t m_Size;
		size_t m_MaxSize;
	};

	template<typename T>
	Pool<T>::Pool(size_t a_MaxSize) {
		m_MaxSize = a_MaxSize;
		m_Alloc.Init(sizeof(T) * m_MaxSize);
		InitNewElements();
	}

	template<typename T>
	Pool<T>::~Pool() {
		m_Alloc.Clear();
	}

	template<typename T>
	void Pool<T>::InitNewElements(size_t a_num) {
		PoolElement<T>* prev = NULL;

		for (size_t i = 0; i < a_num; i++) {
			PoolElement<T> el;
			el.element = BBNew(m_Alloc, T);
			el.nextElement = prev;
			prev = &el;
			m_Size++;
		}
		
		m_Head = prev;
	}

	template<typename T>
	T* Pool<T>::Pop() {
		if (m_Head != NULL) {
			PoolElement<T>* el = m_Head;
			m_Head = m_Head->nextElement;
			return el->element;
		}

		if (m_Size < m_MaxSize) {
			size_t newAmount = (m_MaxSize - m_Size) % 5;
			InitNewElements(newAmount);
			Pop();
		}

		return nullptr;
	}

	template<typename T>
	void Pool<T>::PushFront(T* a_Element) {
		PoolElement<T> el;
		el.element = a_Element;
		el.nextElement = m_Head;
		m_Head = &el;
	}
}