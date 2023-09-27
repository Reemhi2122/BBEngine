#include <gtest/gtest.h>
#include "../BBEngine/BBMemory.h"

TEST(Allocation, ArenaAllocation) 
{
	BBE::Allocators::ArenaAllocator arena;
	arena.Init(1024);
	
	int* xValue = BBNew(arena, int)(20);

	EXPECT_EQ(*xValue, 20);

	BBFree(arena, xValue);
}

TEST(Allocation, StackAllocation) 
{
	BBE::Allocators::StackAllocator arena;
	arena.Init(1024);

	int* xValue = BBNew(arena, int)(20);

	EXPECT_EQ(*xValue, 20);

	BBFree(arena, xValue);
}

TEST(Allocation, PoolAllocator) 
{
	BBE::Allocators::PoolAllocator arena;
	arena.Init(1024, DEFAULT_ALIGNMENT, 16);

	int* xValue = BBNew(arena, int)(20);

	EXPECT_EQ(*xValue, 20);

	BBFree(arena, xValue);
}

TEST(Allocation, FreeListAllocator) 
{
	BBE::Allocators::FreeListAllocator arena;
	arena.Init(1024);

	int* xValue = BBNew(arena, int)(20);

	EXPECT_EQ(*xValue, 20);

	BBFree(arena, xValue);
}

TEST(Allocation, BuddyAllocator) 
{
	BBE::Allocators::BuddyAllocator arena;
	arena.Init(1024);

	int* xValue = BBNew(arena, int)(20);

	EXPECT_EQ(*xValue, 20);

	BBFree(arena, xValue);
}