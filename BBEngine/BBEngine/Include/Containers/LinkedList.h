#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE 
{
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
		LinkedList<T>& operator=(const LinkedList<T>& a_Rhs);

		void Push_Back(T a_Element);
		void Push_Front(T a_Element);
		T Pop_Back();
		T Pop_Front();

		uint32_t Size();

	private:
		LinkedListElement<T>* m_Head;
		LinkedListElement<T>* m_Current;

		uint32_t m_Size = 0;

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
		m_Alloc.Clear();
	}

	template<typename T>
	T LinkedList<T>::operator[](int a_Index)
	{
		return m_Head[a_Index].element;
	}

	template<typename T>
	LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& a_Rhs)
	{
		if (this == &a_Rhs)
			return *this;
	
		m_Head = a_Rhs.m_Head;
		m_Current = a_Rhs.m_Current;
		m_Size = a_Rhs.m_Size;
		m_Alloc = a_Rhs.m_Alloc;

		return *this;
	}

	template<typename T>
	void LinkedList<T>::Push_Back(T a_Element)
	{
		LinkedListElement<T>* newElement = BBNew(m_Alloc, LinkedListElement<T>);
		newElement->element = a_Element;
		newElement->next = nullptr;
		newElement->prev = nullptr;
		m_Size++;

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
		m_Size++;

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
		
		m_Size--;
		T& res = m_Current->element;
		m_Current = m_Current->prev;
		return res;
	}

	template<typename T>
	T LinkedList<T>::Pop_Front()
	{
		BB_Assert(m_Head != NULL, "No more elements in list!");

		m_Size--;
		T& res = m_Head->element;
		m_Head = m_Head->next;
		return res;
	}

	template<typename T>
	inline uint32_t LinkedList<T>::Size()
	{
		return m_Size;
	}
}