#include <gtest/gtest.h>
#include <windows.h>
#include "Thread/ThreadPool.h"

BBE::Allocators::ArenaAllocator m_ArenaAllocator;
BBE::ThreadPool m_ThreadPool = BBE::ThreadPool(8);

void ThreadTest(void*) {
    Sleep(2 * 1000);
    printf("done!");
}

TEST(Thread, ThreadPool) 
{
    for (int i = 0; i < 10; i++) {
        m_ThreadPool.AddTask(ThreadTest);
    }
}