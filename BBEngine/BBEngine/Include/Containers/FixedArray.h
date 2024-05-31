#include "Utility/BBMemory.h"
#include "Logger/Logger.h"

template<typename ArrayType>
class FixedArray 
{


private:
	uint32_t m_Size;
	ArrayType* m_InnerArray;
};