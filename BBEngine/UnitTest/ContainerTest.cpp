#include <gtest/gtest.h>
#include "Containers/Pool.h"
#include "Containers/MemoryPool.h"

TEST(Containers, Pool)
{
	BBE::Pool<int> pool = BBE::Pool<int>(16);

	int* test = pool.Pop();
	
	pool.PushFront(test);
	int* test2 = pool.Pop();
}

TEST(Containers, MemoryPool) 
{
	BBE::MemoryPool<size_t> pool = BBE::MemoryPool<size_t>(4);

	size_t* test = pool.Pop();
	*test = 10;

	size_t* test1 = pool.Pop();
	size_t* test2 = pool.Pop();
	size_t* test3 = pool.Pop();

	pool.PushFront(test);
	pool.PushFront(test1);

	size_t* test4 = pool.Pop();
	size_t* test5 = pool.Pop();
}