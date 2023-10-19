#pragma once
#include "Utility/BBMemory.h"

namespace BBE {

	template<typename T>
	struct LinkedListElement {
		LinkedListElement<T>* next;
		T element;
	};

	template<typename T>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();

		T operator[](int a_Index);

		void Add(T a_Element);
		T Get();

	private:
		LinkedListElement<T>* m_Head;
		LinkedListElement<T>* m_Current;

		Allocators::StackAllocator m_Alloc;
	};

	template<typename T>
	LinkedList<T>::LinkedList() {
		m_Head = nullptr;
		m_Current = nullptr;

		m_Alloc.Init(MBSize);
	}

	template<typename T>
	LinkedList<T>::~LinkedList() {

	}

	template<typename T>
	T LinkedList<T>::operator[](int a_Index)
	{
		return m_Head[a_Index];
	}

	template<typename T>
	void LinkedList<T>::Add(T a_Element)
	{
		LinkedListElement<T>* newElement = BBNew(m_Alloc, LinkedListElement<T>);
		newElement->element = a_Element;
		newElement->next = nullptr;

		if (!m_Head) {
			m_Head = newElement;
			m_Current = m_Head;
			return;
		}

		m_Current->next = newElement;
		m_Current = m_Current->next;
	}

	template<typename T>
	T LinkedList<T>::Get()
	{
		return T();
	}
}