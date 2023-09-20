#include "BuddyAllocator.h"
#include <malloc.h>
#include "../Logger/Logger.h"

namespace BBE {
	namespace Allocators {

		BuddyAllocator::BuddyAllocator()
		{

		}

		BuddyAllocator::~BuddyAllocator()
		{

		}

		void BuddyAllocator::Init(const size_t& a_Size, const size_t& a_ChunkSize)
		{
		}

		void* BuddyAllocator::Alloc(const size_t& a_Size, const size_t& a_Align)
		{
			return nullptr;
		}

		void* BuddyAllocator::Realloc(void* a_Ptr, const size_t& a_OldSize, const size_t& a_NewSize, const size_t& a_Align)
		{
			return nullptr;
		}

		void BuddyAllocator::Free(void* a_Ptr)
		{
		}

		void BuddyAllocator::Clear()
		{
		}

	}
}