#include <gtest/gtest.h>
#include "Containers/Pool.h"
#include "Containers/MemoryPool.h"
#include "Containers/Queue.h"
#include "Containers/LinkedList.h"
#include "Containers/Vector.h"

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

	queue.Push_Back(1);
	queue.Push_Back(2);
	queue.Push_Back(3);
	queue.Push_Back(4);

	int test2 = queue.Get();
	int test3 = queue.Get();
}

TEST(Containers, LinkedList) 
{
	BBE::LinkedList<int> linkedList;

	linkedList.Push_Back(2);
	linkedList.Push_Back(4);
	linkedList.Push_Back(6);
	linkedList.Push_Back(8);

	linkedList.Push_Front(8);

	int x = linkedList.Pop_Front();
	int y = linkedList.Pop_Front();
}

TEST(Containers, Vector)
{
	BBE::Vector<uint32_t> vector;

	uint32_t amount = 16;

	for (int i = 0; i < amount; i++) {
		vector.Push_Back(i);
	}

	uint32_t value0 = vector.Pop_Back();
	uint32_t value1 = vector.Pop_Back();

	vector.Insert(6, 3);
	vector.Remove(8, 2);
}