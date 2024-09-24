#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE
{
	template<typename T>
	class FixedArray
	{
	public:
		FixedArray(uint32_t a_Size);
		~FixedArray();

		T* GetArray();
		uint32_t GetCurrentSize();
		int32_t AddElement(T a_Element);

	private:
		uint32_t m_MaxSize;
		uint32_t m_CurrentSize;
		T* m_InnerArray;
	};

	template<typename ArrayType>
	inline FixedArray<ArrayType>::FixedArray(uint32_t a_Size)
	{
		m_MaxSize = a_Size;
		m_InnerArray = T[a_Size];
	}

	template<typename T>
	inline FixedArray<T>::~FixedArray()
	{
		delete[] m_InnerArray;
		m_InnerArray = nullptr;
	}

	template<typename T>
	inline T* FixedArray<T>::GetArray()
	{
		m_InnerArray;
	}

	template<typename T>
	inline uint32_t FixedArray<T>::GetCurrentSize()
	{
		return m_CurrentSize;
	}

	template<typename T>
	inline int32_t FixedArray<T>::AddElement(T a_Element)
	{
		uint32_t handle;
		if (m_CurrentSize < m_MaxSize)
		{
			m_InnerArray[m_CurrentSize] = a_Element;
			handle = m_CurrentSize++;
			return handle;
		}
		return -1;
	}
}