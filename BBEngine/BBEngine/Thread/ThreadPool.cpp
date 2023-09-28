#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

namespace BBE {

	ThreadPool::ThreadPool(int ThreadCount)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);

		m_ThreadCount = info.dwNumberOfProcessors;
	}

	ThreadPool::~ThreadPool() {

	}

	BBThreadHandle ThreadPool::StartTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam)
	{
		LPDWORD threadId = 0;
		BBThreadHandle handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)a_ThreadFunction, (LPVOID)a_ThreaFunctionParam, 0, threadId);
		return handle;
	}
}