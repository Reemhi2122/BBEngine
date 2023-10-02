#include <gtest/gtest.h>
#include "Utility/BBMemory.h"

constexpr int OneKB = 1024;
constexpr int OneMB = 1024 * OneKB;

constexpr int FORLOOPSIZE = 512;

struct SmallByteStruct {
	char Bytes[128];				// 128 Bytes
};

struct MediumByteStruct {
	char Bytes[4 * OneKB];			// 4 KB
};

struct BigByteStruct {
	char Bytes[OneMB];				// 1 MB
};

TEST(Allocation, ArenaAllocation) 
{
	BBE::Allocators::ArenaAllocator arena;
	arena.Init(OneMB);
	
	SmallByteStruct*	aValue = BBNew(arena, SmallByteStruct);
	MediumByteStruct*	bValue = BBNew(arena, MediumByteStruct);
	BigByteStruct*		cValue = BBNew(arena, BigByteStruct);

	BBFree(arena, cValue);
	BBFree(arena, bValue);
	BBFree(arena, aValue);

	int* test[FORLOOPSIZE];

	for (int i = 0; i < FORLOOPSIZE; i++) {
		test[i] = BBNew(arena, int)(50);
	}

	for (int i = 0; i < FORLOOPSIZE; i++) {
		BBFree(arena, test[i]);
	}
}

TEST(Allocation, StackAllocation) 
{
	BBE::Allocators::StackAllocator stack;
	stack.Init(OneMB);

	SmallByteStruct*	aValue = BBNew(stack, SmallByteStruct);
	MediumByteStruct*	bValue = BBNew(stack, MediumByteStruct);
	BigByteStruct*		cValue = BBNew(stack, BigByteStruct);

	BBFree(stack, cValue);
	BBFree(stack, bValue);
	BBFree(stack, aValue);

	int* test[FORLOOPSIZE];

	for (int i = 0; i < FORLOOPSIZE; i++) {
		test[i] = BBNew(stack, int)(50);
	}

	for (int i = FORLOOPSIZE - 1; i >= 0; i--) {
		BBFree(stack, test[i]);
	}
}

TEST(Allocation, PoolAllocator) 
{
	BBE::Allocators::PoolAllocator pool;
	pool.Init(OneMB * 4, DEFAULT_ALIGNMENT, sizeof(BigByteStruct));

	SmallByteStruct*	aValue = BBNew(pool, SmallByteStruct);
	MediumByteStruct*	bValue = BBNew(pool, MediumByteStruct);
	BigByteStruct*		cValue = BBNew(pool, BigByteStruct);

	BBFree(pool, cValue);
	BBFree(pool, bValue);
	BBFree(pool, aValue);

	pool.Init(OneMB * 4, DEFAULT_ALIGNMENT, sizeof(int));

	int* test[FORLOOPSIZE];

	for (int i = 0; i < FORLOOPSIZE; i++) {
		test[i] = BBNew(pool, int)(50);
	}

	for (int i = 0; i < FORLOOPSIZE; i++) {
		BBFree(pool, test[i]);
	}
}

TEST(Allocation, FreeListAllocator) 
{
	BBE::Allocators::FreeListAllocator freelist;
	freelist.Init(OneMB);

	SmallByteStruct*	aValue = BBNew(freelist, SmallByteStruct);
	MediumByteStruct*	bValue = BBNew(freelist, MediumByteStruct);
	BigByteStruct*		cValue = BBNew(freelist, BigByteStruct);

	BBFree(freelist, cValue);
	BBFree(freelist, bValue);
	BBFree(freelist, aValue);

	int* test[FORLOOPSIZE];

	for (int i = 0; i < FORLOOPSIZE; i++) {
		test[i] = BBNew(freelist, int)(50);
	}

	for (int i = 0; i < FORLOOPSIZE; i++) {
		BBFree(freelist, test[i]);
	}
}

TEST(Allocation, BuddyAllocator) 
{
	BBE::Allocators::BuddyAllocator Buddy;
	Buddy.Init(OneMB);

	SmallByteStruct*	aValue = BBNew(Buddy, SmallByteStruct);
	MediumByteStruct*	bValue = BBNew(Buddy, MediumByteStruct);
	BigByteStruct*		cValue = BBNew(Buddy, BigByteStruct);

	BBFree(Buddy, cValue);
	BBFree(Buddy, bValue);
	BBFree(Buddy, aValue);

	int* test[FORLOOPSIZE];

	for (int i = 0; i < FORLOOPSIZE; i++) {
		test[i] = BBNew(Buddy, int)(50);
	}

	for (int i = 0; i < FORLOOPSIZE; i++) {
		BBFree(Buddy, test[i]);
	}
}