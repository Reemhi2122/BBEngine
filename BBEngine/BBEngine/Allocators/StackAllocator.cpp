#include "StackAllocator.h"

void* BBE::Allocators::StackAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
{
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
