#include <gtest/gtest.h>
#include <windows.h>
#include "Thread/ThreadPool.h"

BBE::Allocators::ArenaAllocator m_ArenaAllocator;
BBE::ThreadPool m_ThreadPool = BBE::ThreadPool(8, 2);

void ThreadTest(void*) {
    Sleep(2 * 1000);
    printf("done!");
}

void StaticThreadTest(void*) {
    Sleep(1 * 1000);

    printf("Static thread called!");
}

TEST(Thread, ThreadPool) 
{
    for (int i = 0; i < 10; i++) {
        m_ThreadPool.AddTask(ThreadTest);
    }

    BBE::BBThreadHandle static_handle = m_ThreadPool.CreateStaticThread(StaticThreadTest);
    m_ThreadPool.DestoryStaticThread(static_handle);
}

