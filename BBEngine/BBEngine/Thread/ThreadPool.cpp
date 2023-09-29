#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

#include "Logger/Logger.h" 

namespace BBE {

	enum class ThreadStatus {
		Idle,
		Working,
		Waiting
	};

	struct Thread {
		ThreadStatus status = ThreadStatus::Idle;
		BBThreadHandle handle = 0;
		DWORD threadId = 0;

		BBThreadHandle StartThread(void(*a_ThreadFunction), void* a_ThreaFunctionParam) {
			BBThreadHandle handle = CreateThread(
				NULL,
				0,
				(LPTHREAD_START_ROUTINE)a_ThreadFunction,
				(LPVOID)a_ThreaFunctionParam,
				0,
				&threadId);

			status = ThreadStatus::Working;
			printf("Thread %d: doing task", (int)threadId);

			return handle;
		}
	};

	ThreadPool::ThreadPool(const uint8_t& AmountOfStaticThreads)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		m_ThreadCount = static_cast<uint8_t>(info.dwNumberOfProcessors);

		if (m_ThreadCount < AmountOfStaticThreads) {
			BB_Assert(0, "Assigning more static threads than threads available");
		}
		
		m_StaticThreadCount = AmountOfStaticThreads;
		m_PoolThreadCount = m_ThreadCount - m_StaticThreadCount;

		m_ThreadAlloc.Init(PageSize);
		m_PoolThreads = BBNewArr(m_ThreadAlloc, m_PoolThreadCount, Thread);
		m_StaticThreads = BBNewArr(m_ThreadAlloc, m_StaticThreadCount, Thread);

	}

	ThreadPool::~ThreadPool()
	{
		BBFreeArr(m_ThreadAlloc, m_PoolThreads);
		BBFreeArr(m_ThreadAlloc, m_StaticThreads);
	}

	BBThreadHandle ThreadPool::AddTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam)
	{
		for (int i = 0; i < m_PoolThreadCount; i++) {
			if (m_PoolThreads[i].status == ThreadStatus::Idle) {
				return m_PoolThreads[i].StartThread(a_ThreadFunction, a_ThreaFunctionParam);
			}
		}

		return NULL;
	}

	uint8_t ThreadPool::GetNumberOfStaticThreads() const
	{
		return m_StaticThreadCount;
	}

	uint8_t ThreadPool::GetNumberOfPoolThreads() const
	{
		return m_PoolThreadCount;
	}
}