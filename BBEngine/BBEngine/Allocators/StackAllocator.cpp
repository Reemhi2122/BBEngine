#include "StackAllocator.h"

BBE::Allocators::StackAllocator::StackAllocator()
{
	m_Stack.buf = nullptr;
	m_Stack.bufSize = 0u;
	m_Stack.curOffset = 0u;
	m_Stack.prevOffset = 0u;
}

void BBE::Allocators::StackAllocator::Init(const size_t& a_Size) noexcept
{
	m_Stack.buf = malloc(a_Size);
	m_Stack.bufSize = a_Size;
	m_Stack.curOffset = 0u;
	m_Stack.prevOffset = 0u;
}

void* BBE::Allocators::StackAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
{
	BB_Assert(IsPowerOfTwo(a_Align), "Stack allocator align is not a power of two");
	BB_Assert(a_Align > 128, "Stack allocator alignment can't be more than 128");

	uintptr_t currAddr = (uintptr_t)m_Stack.buf + (uintptr_t)m_Stack.curOffset;
	size_t padding = CalculateAlignOffset(currAddr, a_Align, sizeof(StackHeader));
	
	BB_Assert((m_Stack.curOffset + padding + a_Size < m_Stack.bufSize), "Stack allocator is out of memory");

	m_Stack.prevOffset = m_Stack.curOffset;

	uintptr_t nextAddr = currAddr + padding;
	StackHeader* header = reinterpret_cast<StackHeader*>(nextAddr - sizeof(StackHeader));
	header->padding = static_cast<size_t>(padding);
	header->prevOffset = m_Stack.prevOffset;

	m_Stack.curOffset += padding + a_Size;

	return memset(reinterpret_cast<void*>(nextAddr), 0, a_Size);
}

void* BBE::Allocators::StackAllocator::Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
{
	if (a_Ptr == NULL)
		return Alloc(a_NewSize, a_Align);

	if (a_NewSize == 0) {
		Free(a_Ptr);
		return NULL;
	}

	size_t minSize = a_OldSize < a_NewSize ? a_OldSize : a_NewSize;

	uintptr_t start = reinterpret_cast<uintptr_t>(m_Stack.buf);
	uintptr_t currAddr = (uintptr_t)a_Ptr;

	BB_Assert((currAddr > start || currAddr < m_Stack.bufSize), "Stack allocator out of bounds!");

	if (currAddr >= start + m_Stack.curOffset)
		return NULL;

	if (a_OldSize == a_NewSize) 
		return a_Ptr;

	//if (m_Stack.curOffset != currAddr + a_OldSize) {
	//	size_t nextPadding = CalculateAlignOffset(currAddr + a_OldSize, a_Align, sizeof(StackHeader));
	//	StackHeader* curHeader = reinterpret_cast<StackHeader*>(currAddr - sizeof(StackHeader));
	//	StackHeader* nextHeader = reinterpret_cast<StackHeader*>(currAddr + nextPadding - sizeof(StackHeader));
	//	nextHeader->prevOffset = currAddr - curHeader->prevOffset;
	//	nextHeader->padding = nextPadding;
	//}


	void* newPtr = StackAllocator::Alloc(a_NewSize, a_Align);
	memmove(newPtr, a_Ptr, minSize);
	return newPtr;
}

void BBE::Allocators::StackAllocator::Free(void* a_Ptr)
{
	if (a_Ptr == NULL)
		return;

	uintptr_t start = reinterpret_cast<uintptr_t>(m_Stack.buf);
	uintptr_t currAddr = (uintptr_t)a_Ptr;

	BB_Assert((currAddr < start && currAddr > m_Stack.bufSize), "Stack allocator out of bounds!");

	if (currAddr >= start + m_Stack.curOffset)
		return;

	StackHeader* header = reinterpret_cast<StackHeader*>(currAddr - sizeof(StackAllocator));
	
	uintptr_t prevOffset = currAddr - header->padding - start;

	BB_Assert((prevOffset != header->prevOffset), "Out of order stack allocation!");

	m_Stack.curOffset = m_Stack.prevOffset;
	m_Stack.prevOffset = header->prevOffset;

}

void BBE::Allocators::StackAllocator::Clear()
{
	m_Stack.curOffset = 0;
}
