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
	BB_Assert(a_Align > 128, "Stack allocator alignment can't be more than 128");

	uintptr_t currAddr = (uintptr_t)m_Stack.buf + (uintptr_t)m_Stack.offset;
	size_t padding = CalculateAlignOffset(currAddr, a_Align, 8);
	
	BB_Assert((m_Stack.offset + padding + a_Size > m_Stack.bufSize), "Stack allocator is out of memory");

	uintptr_t nextAddr = currAddr + padding;
	StackHeader* header = reinterpret_cast<StackHeader*>(nextAddr - sizeof(StackAllocator));
	header->padding = static_cast<uint8_t>(padding);

	m_Stack.offset += padding + a_Size;

	return memset(reinterpret_cast<void*>(nextAddr), 0, a_Size);
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
