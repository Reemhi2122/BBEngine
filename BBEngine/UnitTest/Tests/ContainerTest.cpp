#include <gtest/gtest.h>
#include "Containers/Pool.h"
#include "Containers/MemoryPool.h"
#include "Containers/Queue.h"
#include "Containers/LinkedList.h"

TEST(Containers, Pool)
{
	BBE::Pool<int> pool = BBE::Pool<int>(4);

	int* test = pool.Pop();
	int* test1 = pool.Pop();
	int* test2 = pool.Pop();
	int* test3 = pool.Pop();
	
	pool.PushFront(test1);
	
	int* test6 = pool.Pop();
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

TEST(Containers, Queue)
{
	BBE::Queue<int> queue = BBE::Queue<int>(5);

	queue.Add(1);
	queue.Add(2);
	queue.Add(3);
	queue.Add(4);

	int test2 = queue.Get();
	int test3 = queue.Get();
}

TEST(Containers, LinkedList) 
{
	BBE::LinkedList<int> linkedList;

	linkedList.Add(2);
	linkedList.Add(4);
	linkedList.Add(6);
	linkedList.Add(8);

	int x = linkedList.Get();
	int y = linkedList.Get();
}