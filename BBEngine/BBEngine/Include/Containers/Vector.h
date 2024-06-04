#pragma once
#include <stdint.h>

#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE {

	template<typename T>
	class Vector {
	public:
		Vector(uint32_t a_Size = 8);
		~Vector();

		Vector<T>& operator=(const Vector<T>& a_Rhs);
		T operator[](uint32_t a_Index);

		void Push_Back(T a_Element);
		T Pop_Back();
		T At(uint32_t a_Index) const;

		void Remove(uint32_t a_Index, uint32_t a_Length = 1);
		void Insert(uint32_t a_Index, T a_Element);

		uint32_t Size();

	private:
		void Resize();

		Allocators::FreeListAllocator m_Alloc;
		uint32_t m_ArraySize = 0u;
		uint32_t m_CurPos = 0u;
		T* m_Data;
	};

	template<typename T>
	Vector<T>::Vector(uint32_t a_Size)
	{
		m_ArraySize = a_Size;
		m_CurPos = 0u;
		m_Alloc.Init(a_Size * 128);
		m_Data = BBNewArr(m_Alloc, a_Size, T);
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		m_Alloc.Clear();
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& a_Rhs)
	{
		if (this == &a_Rhs) {
			return *this;
		}

		m_Size = a_Rhs.m_Size;
		m_Data = a_Rhs.m_Data;

		return *this;
	}

	template<typename T>
	T Vector<T>::operator[](uint32_t a_Index)
	{
		BB_Assert((a_Index < m_CurPos), "Trying to access data outside of vector range!");
		return m_Data[a_Index];
	}

	template<typename T>
	void Vector<T>::Push_Back(T a_Element)
	{
		uint32_t newSize = m_CurPos + 1;
		if(newSize > m_ArraySize){
			Resize();
		}

		m_Data[m_CurPos] = a_Element;
		m_CurPos = newSize;
	}

	template<typename T>
	T Vector<T>::Pop_Back()
	{
		uint32_t lastpos = m_CurPos - 1;
		T res = m_Data[lastpos];
		m_Data[lastpos] = {};
		m_CurPos--;
		return res;
	}

	template<typename T>
	T Vector<T>::At(uint32_t a_Index) const
	{
		return m_Data[a_Index];
	}

	template<typename T>
	void Vector<T>::Remove(uint32_t a_Index, uint32_t a_Length)
	{
		T* temp = BBNewArr(m_Alloc, m_ArraySize, T);
		uint32_t cutoff = a_Index - 1;
		memcpy(temp, m_Data, cutoff * sizeof(T));
		uint32_t sizeleft = (cutoff + a_Length);
		memcpy(temp + cutoff, m_Data + sizeleft, ((m_ArraySize - sizeleft) * sizeof(T)));

		BBFreeArr(m_Alloc, m_Data);
		m_Data = temp;
		m_CurPos -= a_Length;
	}

	template<typename T>
	void Vector<T>::Insert(uint32_t a_Index, T a_Element)
	{
		if (m_CurPos + 1 > m_ArraySize) {
			Resize();
		}

		T* temp = BBNewArr(m_Alloc, m_ArraySize, T);
		uint32_t cutoff = a_Index;
		memcpy(temp, m_Data, cutoff * sizeof(T));
		temp[a_Index] = a_Element;
		uint32_t newIndex = a_Index + 1;
		uint32_t size = m_ArraySize - newIndex;
		memcpy(temp + newIndex, m_Data + a_Index, size * sizeof(T));

		BBFreeArr(m_Alloc, m_Data);
		m_Data = temp;
		m_CurPos++;
	}

	template<typename T>
	uint32_t Vector<T>::Size() {
		return m_CurPos;
	}

	template<typename T>
	void Vector<T>::Resize()
	{
		uint32_t newSize = m_ArraySize * 2;
		T* temp = BBNewArr(m_Alloc, newSize, T);

		memcpy(temp, m_Data, m_ArraySize * sizeof(T));
		
		BBFreeArr(m_Alloc, m_Data);

		m_Data = temp;
		m_ArraySize = newSize;
	}
}