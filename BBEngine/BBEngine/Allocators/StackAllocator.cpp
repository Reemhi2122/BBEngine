#include "StackAllocator.h"

BBE::Allocators::StackAllocator::StackAllocator()
{
	m_Stack.buf = nullptr;
	m_Stack.bufSize = 0u;
	m_Stack.offset = 0u;
}

void BBE::Allocators::StackAllocator::Init(const size_t& a_Size) noexcept
{
	m_Stack.buf = malloc(a_Size);
	m_Stack.bufSize = a_Size;
	m_Stack.offset = 0u;
}

void* BBE::Allocators::StackAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
{
	BB_Assert(IsPowerOfTwo(a_Align), "Align is not a power of two");



	return nullptr;
}

void* BBE::Allocators::StackAllocator::Realloc(void* a_OldData, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
{
	return nullptr;
}

void BBE::Allocators::StackAllocator::Free(void* ptr)
{
}

void BBE::Allocators::StackAllocator::Clear()
{
}
