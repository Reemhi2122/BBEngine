#pragma once
#include "Utility/BBMemory.h"

namespace BBE {

	template<typename T>
	struct QueueElement {
		T element;
		QueueElement<T>* next;
	};

	template<typename T>
	class Queue
	{
	public:
		Queue();
		~Queue();

		void Add(T a_Element);
		T Get();

	private:
		QueueElement<T>* m_Head;
		QueueElement<T>* m_Tail;

		Allocators::PoolAllocator m_Alloc;
	};

	template<typename T>
	Queue<T>::Queue() {
		m_Head = nullptr;
		m_Tail = nullptr;

		m_Alloc.Init(sizeof(T) * 32, DEFAULT_ALIGNMENT, sizeof(T));
	}

	template<typename T>
	Queue<T>::~Queue() {
		m_Head = nullptr;
		m_Tail = nullptr;
	}

	template<typename T>
	inline void Queue<T>::Add(T a_Element)
	{
		QueueElement<T>* el = BBNew(m_Alloc, QueueElement<T>);
		el->element = a_Element;

		if (m_Head == NULL) {
			el->next = nullptr;
			m_Head = el;
			m_Tail = el;
			return;
		}

		m_Tail->next = el;
		m_Tail = el;
	}

	template<typename T>
	inline T Queue<T>::Get()
	{
		QueueElement<T>* el = m_Head;
		T ptr = m_Head->element;
		m_Head = m_Head->next;
		BBFree(m_Alloc, el);
		return ptr;
	}
}