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

		void BuddyAllocator::Init(const size_t& a_Size, const size_t a_Allignment, const size_t& a_ChunkSize)
		{
			BB_Assert(IsPowerOfTwo(a_Size), "Size is not in a power of two");


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

		Buddy* BuddyAllocator::GetBuddy(const Buddy* a_Buddy)
		{
			return reinterpret_cast<Buddy*>(Pointer::Add(a_Buddy, a_Buddy->blockSize));
		}

		Buddy* BuddyAllocator::SplitBuddy(Buddy* a_buddy, const size_t a_Size)
		{
			if (a_buddy == NULL || a_Size == 0)
				return NULL;
				
			while (a_buddy->blockSize > a_Size)
			{
				size_t sz = a_buddy->blockSize >> 1;
				a_buddy->blockSize = sz;
				a_buddy = GetBuddy(a_buddy);
				a_buddy->blockSize = sz;
				a_buddy->isFree = true;
			}

			if (a_Size <= a_buddy->blockSize) {
				return a_buddy;
			}
		}
		Buddy* BuddyAllocator::FindBestBuddy(Buddy* a_Head, Buddy* a_Tail, const size_t& a_Size)
		{
			Buddy* bestBlock = NULL;
			Buddy* block = a_Head;
			Buddy* buddy = GetBuddy(block);

			if (buddy == a_Tail && block->isFree)
				return SplitBuddy(block, a_Size);

			//Get best fit buddy for block
			while (block < a_Tail && buddy < a_Tail) {
				
				//Didn't do the optimization here yet

				if (block->isFree && 
					a_Size < block->blockSize &&
					(bestBlock == NULL || bestBlock->blockSize > block->blockSize)) {
					bestBlock = block;
				}

				if (buddy->isFree &&
					a_Size < buddy->blockSize &&
					(bestBlock == NULL || bestBlock->blockSize > buddy->blockSize)) {
					bestBlock = buddy;
				}

				//Not fully understand this yet..
				if (block->blockSize <= buddy->blockSize) {
					block = GetBuddy(buddy);
					if (block < a_Tail) {
						buddy = GetBuddy(block);
					}
				}
				else {
					block = buddy;
					buddy = GetBuddy(buddy);
				}
			}

			if (bestBlock != NULL) {
				return SplitBuddy(bestBlock, a_Size);
			}

			return NULL;
		}
	}
}