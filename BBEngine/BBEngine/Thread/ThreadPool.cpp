#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

#include "Logger/Logger.h" 

#define MIMTHREADS 8

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

		InitializeThreads(MIMTHREADS);
	}

	ThreadPool::~ThreadPool()
	{
	}

	BBThreadHandle ThreadPool::AddTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam)
	{


		return NULL;
	}

	void ThreadPool::InitializeThreads(const uint8_t& a_Count)
	{
		for (int i = 0; i < 8; i++) {
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
		}
	}
}