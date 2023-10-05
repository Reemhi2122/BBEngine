#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

#include "Logger/Logger.h" 

namespace BBE {

	uint8_t ThreadPool::OccupiedThreads = 0;
	constexpr uint8_t systemThreads = 1;

	void ThreadPoolSystem(void* a_SthdDesc) {
		SystemThreadDesc* sthdDesc = reinterpret_cast<SystemThreadDesc*>(a_SthdDesc);

		if (sthdDesc->sthdQueue->IsEmpty()) {
			return;
		}

		ThreadDesc* thdDesc = sthdDesc->sthdPool->Pop();
		if (thdDesc != NULL && thdDesc->thdStatus == ThreadStatus::Waiting) {
			TaskDesc tskDesc = sthdDesc->sthdQueue->Get();
			thdDesc->thdFunction = tskDesc.tskFunction;
			thdDesc->thdParams = tskDesc.tskParam;
			thdDesc->thdStatus = ThreadStatus::Working;
			return;
		}

		for (size_t i = 0; i < sthdDesc->sthdPoolSize; i++)
		{
			ThreadDesc* trd = &sthdDesc->sthdPoolArray[i];
			if (trd != NULL && trd->thdStatus == ThreadStatus::Idle) {
				trd->thdStatus = ThreadStatus::Waiting;
				sthdDesc->sthdPool->PushFront(trd);
			}
		}
	}

	DWORD WINAPI ThreadFunction(LPVOID lpParam) {
		ThreadDesc* thread = reinterpret_cast<ThreadDesc*>(lpParam);

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

	DWORD WINAPI ThreadStaticFunction(LPVOID lpParam) {
		ThreadDesc* thread = reinterpret_cast<ThreadDesc*>(lpParam);

		while (thread->thdStatus != ThreadStatus::Terminate)
		{
			thread->thdFunction(thread->thdParams);
		}

		CloseHandle(thread->thdHandle);
		return 0;
	}

	ThreadPool::ThreadPool(const uint8_t& a_ThreadPoolCount)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		uint8_t SystemThreadCount = static_cast<uint8_t>(info.dwNumberOfProcessors);

		m_ThreadAlloc.Init(a_ThreadPoolCount * sizeof(ThreadDesc));
		
		m_PoolThreadCount = a_ThreadPoolCount;
		m_ThreadCount = m_PoolThreadCount + systemThreads;
		uint8_t totalThreads = OccupiedThreads + m_ThreadCount;

		BB_Assert((totalThreads < SystemThreadCount), "Assigning more threads than threads available");

		InitializePoolThreads(a_ThreadPoolCount);
		CreatePoolSystemThread();
	}

	ThreadPool::~ThreadPool()
	{
		m_SystemThread.thdStatus = ThreadStatus::Terminate;

		for (size_t i = 0; i < m_PoolThreadCount; i++) {
			m_PoolThreads[i].thdStatus = ThreadStatus::Terminate;
		}

		OccupiedThreads -= m_ThreadCount;
		BBFreeArr(m_ThreadAlloc, m_PoolThreads);
	}

	BBTaskHandle ThreadPool::AddTask(void (*a_void)(void*), void* a_ThreadFunctionParam)
	{
		TaskDesc task;
		task.tskStatus = TaskStatus::Queue;
		task.tskFunction = a_void;
		task.tskParam = a_ThreadFunctionParam;
		return m_TaskQueue.Add(task);
	}

	void ThreadPool::InitializePoolThreads(const uint8_t& a_Count)
	{
		m_PoolThreads = BBNewArr(m_ThreadAlloc, a_Count, ThreadDesc);

		for (int i = 0; i < a_Count; i++) {
			ThreadDesc* pram = &m_PoolThreads[i];
			pram->thdStatus = ThreadStatus::Idle;
			pram->thdFunction = nullptr;
			pram->thdParams = nullptr;
			pram->thdHandle = CreateThread(
				NULL,
				0,
				ThreadFunction,
				pram,
				0,
				NULL);

			m_Pool.PushFront(pram);
		}
	}

	void ThreadPool::CreatePoolSystemThread()
	{
		m_SystemDesc.sthdQueue = &m_TaskQueue;
		m_SystemDesc.sthdPool = &m_Pool;
		m_SystemDesc.sthdPoolSize = m_PoolThreadCount;
		m_SystemDesc.sthdPoolArray = m_PoolThreads;

		m_SystemThread.thdStatus = ThreadStatus::Working;
		m_SystemThread.thdFunction = ThreadPoolSystem;
		m_SystemThread.thdParams = &m_SystemDesc;
		m_SystemThread.thdHandle = CreateThread(
			NULL,
			0,
			ThreadStaticFunction,
			&m_SystemThread,
			0,
			NULL);
	}
}