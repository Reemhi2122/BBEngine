#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

#include "Logger/Logger.h" 

constexpr uint8_t systemThreads = 1;

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

	ThreadPool::ThreadPool(const uint8_t& a_DynamicThreads)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		m_ThreadCount = static_cast<uint8_t>(info.dwNumberOfProcessors);

		int totalThreads = a_DynamicThreads + OccupiedThreads + systemThreads;

		BB_Assert(totalThreads < m_ThreadCount, "Assigning more static threads than threads available");

		InitializePoolThreads(a_DynamicThreads);
	}

	ThreadPool::~ThreadPool()
	{
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
		for (int i = 0; i < 8; i++) {
			ThreadDesc pram;
			pram.thdStatus = ThreadStatus::Idle;
			pram.thdFunction = nullptr;
			pram.thdParams = nullptr;

			m_Pool.PushFront(CreateThread(
				NULL,
				0,
				ThreadFunction,
				&pram,
				0,
				NULL));
		}
	}
}