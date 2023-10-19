#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE {

	template<typename T>
	struct LinkedListElement {
		LinkedListElement<T>* next;
		LinkedListElement<T>* prev;
		T element;
	};

	template<typename T>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();

		T operator[](int a_Index);

		void Push_Back(T a_Element);
		void Push_Front(T a_Element);
		T Pop_Back();
		T Pop_Front();

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
	void LinkedList<T>::Push_Back(T a_Element)
	{
		LinkedListElement<T>* newElement = BBNew(m_Alloc, LinkedListElement<T>);
		newElement->element = a_Element;
		newElement->next = nullptr;
		newElement->prev = nullptr;

		if (!m_Head) {
			m_Head = newElement;
			m_Current = m_Head;
			return;
		}

		newElement->prev = m_Current;
		m_Current->next = newElement;
		m_Current = m_Current->next;
	}

	template<typename T>
	void LinkedList<T>::Push_Front(T a_Element)
	{
		LinkedListElement<T>* newElement = BBNew(m_Alloc, LinkedListElement<T>);
		newElement->element = a_Element;
		newElement->next = nullptr;
		newElement->prev = nullptr;

		if (!m_Head) {
			m_Head = newElement;
			m_Current = m_Head;
			return;
		}

		newElement->next = m_Head;
		m_Head->prev = newElement;
		m_Head = newElement;
	}

	template<typename T>
	T LinkedList<T>::Pop_Back()
	{
		BB_Assert(m_Current != NULL, "No more elements in list!");
	
		T& res = m_Current->element;
		m_Current = m_Current->prev;
		return res;
	}

	template<typename T>
	T LinkedList<T>::Pop_Front()
	{
		BB_Assert(m_Head != NULL, "No more elements in list!");

		T& res = m_Head->element;
		m_Head = m_Head->next;
		return res;
	}
}