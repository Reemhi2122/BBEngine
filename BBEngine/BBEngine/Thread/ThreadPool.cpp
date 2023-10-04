#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

#include "Logger/Logger.h" 

constexpr uint8_t systemThreads = 1;
uint8_t BBE::ThreadPool::OccupiedThreads = 0;

namespace BBE {

	struct ThreadDesc {
		ThreadStatus	thdStatus;
		BBThreadHandle	thdHandle;
		void			(*thdFunction)(void*);
		void*			thdParams;
	};

	DWORD WINAPI ThreadFunction(LPVOID lpParam) {
		ThreadDesc* thread = static_cast<ThreadDesc*>(lpParam);

		while (thread->thdStatus != ThreadStatus::Terminate)
		{
			if (thread->thdStatus == ThreadStatus::Working) {
				thread->thdFunction(thread->thdParams);
				thread->thdFunction = nullptr;
				thread->thdParams = nullptr;
				thread->thdStatus = ThreadStatus::Idle;
			}
		}

		CloseHandle(thread->thdHandle);
		return 0;
	}

	ThreadPool::ThreadPool(const uint8_t& a_ThreadPoolCount)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		uint8_t SystemThreadCount = static_cast<uint8_t>(info.dwNumberOfProcessors);

		m_ThreadCount = a_ThreadPoolCount + systemThreads;
		uint8_t totalThreads = OccupiedThreads + m_ThreadCount;

		BB_Assert((totalThreads < SystemThreadCount), "Assigning more threads than threads available");

		InitializePoolThreads(a_ThreadPoolCount);
	}

	ThreadPool::~ThreadPool()
	{
		OccupiedThreads -= m_ThreadCount;
	}

	BBTaskHandle ThreadPool::AddTask(void(*a_ThreadFunction), void* a_ThreadFunctionParam)
	{
		ThreadTaskDesc task;
		task.tskStatus = TaskStatus::Queue;
		task.tskFunction = a_ThreadFunction;
		task.tskParam = a_ThreadFunctionParam;
		return TaskQueue.Add(task);
	}

	void ThreadPool::InitializePoolThreads(const uint8_t& a_Count)
	{
		for (int i = 0; i < a_Count; i++) {
			ThreadDesc pram;
			pram.thdStatus = ThreadStatus::Idle;
			pram.thdFunction = nullptr;
			pram.thdParams = nullptr;
			pram.thdHandle = CreateThread(
				NULL,
				0,
				ThreadFunction,
				&pram,
				0,
				NULL);

			m_Pool.PushFront(&pram.thdHandle);
		}
	}
}