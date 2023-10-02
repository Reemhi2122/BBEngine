#include <gtest/gtest.h>
#include "Containers/Pool.h"

TEST(Containers, Pool)
{
	BBE::Pool<int> pool = BBE::Pool<int>(32);

	int* test = pool.Pop();
	*test = 10;

	pool.PushFront(test);
	int* test2 = pool.Pop();

	printf("%d", *test2);
}