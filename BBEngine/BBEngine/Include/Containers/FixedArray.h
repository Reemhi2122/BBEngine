#pragma once
#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

template<typename ArrayType>
class FixedArray
{
public:
	FixedArray(uint32_t a_Size);
	~FixedArray();

	ArrayType* GetArray();
	uint32_t GetCurrentSize();
	int32_t AddElement(ArrayType a_Element);

private:
	uint32_t m_MaxSize;
	uint32_t m_CurrentSize;
	ArrayType* m_InnerArray;
};

template<typename ArrayType>
inline FixedArray<ArrayType>::FixedArray(uint32_t a_Size)
{
	m_MaxSize = a_Size;
	m_InnerArray = new ArrayType[a_Size];
}

template<typename ArrayType>
inline FixedArray<ArrayType>::~FixedArray()
{
	delete[] m_InnerArray;
	m_InnerArray = nullptr;
}

template<typename ArrayType>
inline ArrayType* FixedArray<ArrayType>::GetArray()
{
	m_InnerArray;
}

template<typename ArrayType>
inline uint32_t FixedArray<ArrayType>::GetCurrentSize()
{
	return m_CurrentSize;
}

template<typename ArrayType>
inline int32_t FixedArray<ArrayType>::AddElement(ArrayType a_Element)
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