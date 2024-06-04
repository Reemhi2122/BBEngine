#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

template<typename ArrayType>
class FixedArray 
{
public:
	FixedArray(uint32_t a_Size) 
	{
		m_MaxSize = a_Size;
	}

	ArrayType* GetArray() 
	{
		m_InnerArray;
	}

	uint32_t GetCurrentSize() 
	{
		return m_CurrentSize;
	}

	void AddElement(ArrayType a_Element) {
		if (m_CurrentSize < m_MaxSize) {
			m_CurrentSize++;
			m_InnerArray[m_CurrentSize] = a_Element;
		}
	}

private:
	uint32_t m_MaxSize;
	uint32_t m_CurrentSize;
	ArrayType* m_InnerArray;
};