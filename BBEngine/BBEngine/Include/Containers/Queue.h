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
		Queue(uint32_t a_Size);
		~Queue();

		Queue<T>& operator=(const Queue<T>& a_Rhs);

		void* Add(T a_Element);
		T Get();

		bool IsEmpty();

	private:
		QueueElement<T>* m_Head;
		QueueElement<T>* m_Tail;

		Allocators::PoolAllocator m_Alloc;
	};

	template<typename T>
	Queue<T>::Queue(uint32_t a_Size) {
		m_Head = nullptr;
		m_Tail = nullptr;

		m_Alloc.Init(sizeof(QueueElement<T>) * a_Size, DEFAULT_ALIGNMENT, sizeof(T));
	}

	template<typename T>
	Queue<T>::~Queue() {
		m_Head = nullptr;
		m_Tail = nullptr;
	}

	template<typename T>
	inline Queue<T>& Queue<T>::operator=(const Queue<T>& a_Rhs)
	{
		if (this == &a_Rhs) {
			return *this;
		}

		m_Head = a_Rhs.m_Head;
		m_Alloc = a_Rhs.m_Alloc;
		m_Tail = a_Rhs.m_Tail;

		return *this;
	}

	template<typename T>
	void* Queue<T>::Add(T a_Element)
	{
		QueueElement<T>* el = BBNew(m_Alloc, QueueElement<T>);
		el->element = a_Element;

		if (m_Head == NULL) {
			el->next = nullptr;
			m_Head = el;
			m_Tail = el;
			return m_Tail;
		}

		m_Tail->next = el;
		m_Tail = el;

		return m_Tail;
	}

	template<typename T>
	T Queue<T>::Get()
	{
		QueueElement<T>* el = m_Head;
		T ptr = m_Head->element;
		m_Head = m_Head->next;
		BBFree(m_Alloc, el);
		return ptr;
	}

	template<typename T>
	bool Queue<T>::IsEmpty()
	{
		return m_Head == nullptr;
	}
}