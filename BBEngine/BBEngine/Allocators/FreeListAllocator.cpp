#include "FreeListAllocator.h"

BBE::Allocators::FreeListAllocator::FreeListAllocator()
{
	m_FreeList.buffer = NULL;
	m_FreeList.head = NULL;
	m_FreeList.policy = PlacementPolicy::FindFirst;
	m_FreeList.size = 0u;
	m_FreeList.used = 0u;
}

BBE::Allocators::FreeListAllocator::~FreeListAllocator()
{
	m_FreeList.buffer = NULL;
	m_FreeList.head = NULL;
}

void BBE::Allocators::FreeListAllocator::Init(const size_t& a_Size, const size_t& a_ChunkSize)
{

}

void* BBE::Allocators::FreeListAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
{
	return nullptr;
}

void* BBE::Allocators::FreeListAllocator::Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
{
	return nullptr;
}

void BBE::Allocators::FreeListAllocator::Free(void* a_Ptr)
{

}

void BBE::Allocators::FreeListAllocator::Clear()
{

}