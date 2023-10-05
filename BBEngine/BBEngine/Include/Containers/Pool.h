#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE {

	template<typename T>
	struct PoolElement {
		T* element;
		PoolElement<T>* nextElement;
	};

	template<typename T>
	class Pool {
	public:
		Pool(size_t a_MaxSize, size_t a_StartSize = 0);
		~Pool();

		T* Pop();
		void PushFront(T* a_Element);

	private:
		void InitNewElements(size_t a_num);

		PoolElement<T>* m_Head;
		Allocators::ArenaAllocator m_ArenaAlloc;
		Allocators::StackAllocator m_StackAlloc;
		size_t m_Size;
		size_t m_MaxSize;
	};


	template<typename T>
	Pool<T>::Pool(size_t a_MaxSize, size_t a_StartSize) {
		m_Head = nullptr;
		m_Size = 0;
		m_MaxSize = a_MaxSize;

		m_ArenaAlloc.Init(sizeof(T) * m_MaxSize);
		m_StackAlloc.Init(sizeof(T) * m_MaxSize);

		InitNewElements(a_StartSize);
	}

	template<typename T>
	Pool<T>::~Pool() {
		m_ArenaAlloc.Clear();
	}

	template<typename T>
	void Pool<T>::InitNewElements(size_t a_num) {
		
		for (size_t i = 0; i < a_num; i++) {
			PoolElement<T>* el = BBNew(m_StackAlloc, PoolElement<T>);
			el->element = BBNew(m_ArenaAlloc, T);
			el->nextElement = m_Head;
			m_Head = el;
			m_Size++;
		}
	}

	template<typename T>
	T* Pool<T>::Pop() {
		if (m_Head != NULL) {
			PoolElement<T>* el = m_Head;
			m_Head = m_Head->nextElement;
	
			T* value = el->element;
			BBFree(m_StackAlloc, el);
			return value;
		}

		//if (m_Size < m_MaxSize) {
		//	size_t newAmount = (m_MaxSize - m_Size) % 5;
		//	InitNewElements(newAmount);
		//	Pop();
		//}

		return nullptr;
	}

	template<typename T>
	void Pool<T>::PushFront(T* a_Element) {
		PoolElement<T>* el = BBNew(m_StackAlloc, PoolElement<T>);
		el->element = a_Element;
		el->nextElement = m_Head;
		m_Head = el;
	}
}