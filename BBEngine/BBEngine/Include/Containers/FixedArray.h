#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

namespace BBE
{
	typedef int32_t FA_HANDLE;

	template<typename Type>
	class FixedArray
	{
	public:
		FixedArray(uint32_t a_Size);
		~FixedArray();

		Type* GetArray();
		uint32_t GetCurrentSize();
		
		FA_HANDLE	AddElement(Type a_Element);
		Type*		GetElement(FA_HANDLE a_Handle);

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

	template<typename Type>
	inline FixedArray<Type>::~FixedArray()
	{
		delete[] m_InnerArray;
		m_InnerArray = nullptr;
	}

	template<typename Type>
	inline Type* FixedArray<Type>::GetArray()
	{
		return m_InnerArray;
	}

	template<typename Type>
	inline uint32_t FixedArray<Type>::GetCurrentSize()
	{
		return m_CurrentSize;
	}

	template<typename Type>
	inline FA_HANDLE FixedArray<Type>::AddElement(Type a_Element)
	{
		FA_HANDLE handle = -1;
		for (uint32_t i = 0; i < m_MaxSize; i++)
		{
			if (m_InnerArray[i] == nullptr)
			{
				m_InnerArray[i] = a_Element;
				handle = i;
				return handle;
			}
		}
		return handle;
	}

	template<typename Type>
	inline Type* FixedArray<Type>::GetElement(FA_HANDLE a_Handle)
	{
		return m_InnerArray[a_Handle];
	}
}