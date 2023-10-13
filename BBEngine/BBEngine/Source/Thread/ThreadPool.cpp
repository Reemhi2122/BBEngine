#include "Thread/ThreadPool.h"
#include <windows.h>
#include <iostream>

#include "Windows.h"

#include "Logger/Logger.h" 

#define UnprocessedThread 0

uint32_t* m_TasksHandled;

namespace BBE {

	uint8_t ThreadPool::OccupiedThreads = 0;
	constexpr uint8_t SYSTEM_THREADS = 1;

	void ThreadPoolSystem(void* a_SthdDesc) {
		SystemThreadDesc* sthdDesc = reinterpret_cast<SystemThreadDesc*>(a_SthdDesc);

		if (sthdDesc->sthdQueue->IsEmpty()) {
			return;
		}

		ThreadDesc* thdDesc = sthdDesc->sthdPool->Pop();
		if (thdDesc != NULL && thdDesc->thdStatus == ThreadStatus::Waiting) {
			TaskDesc tskDesc = sthdDesc->sthdQueue->Get();

			thdDesc->thdTskHandle = &tskDesc;
			thdDesc->thdFunction = tskDesc.tskFunction;
			thdDesc->thdParams = tskDesc.tskParam;
			thdDesc->thdStatus = ThreadStatus::Working;
			*tskDesc.tskHandle = ((uint64_t)sthdDesc->sthdTasksHandled[thdDesc->thdId] + 1) << 32 | thdDesc->thdId;
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
				thread->thdTskHandle = nullptr;
				thread->thdFunction = nullptr;
				thread->thdParams = nullptr;
				m_TasksHandled[thread->thdId] = m_TasksHandled[thread->thdId] + 1;
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

	ThreadPool::ThreadPool()
	{
		m_PoolThreads = nullptr;
		m_StaticThreads = nullptr;
		m_SystemDesc = SystemThreadDesc();
		m_SystemThread = ThreadDesc();
		m_TasksHandled = nullptr;
	}

	ThreadPool::ThreadPool(const uint8_t& a_ThreadPoolCount, const uint8_t& a_StaticThreadCount)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		uint8_t SystemThreadCount = static_cast<uint8_t>(info.dwNumberOfProcessors);

		m_PoolThreadCount = a_ThreadPoolCount;
		m_StaticThreadCount = a_StaticThreadCount;
		m_ThreadCount = m_PoolThreadCount + m_StaticThreadCount + SYSTEM_THREADS;
		uint8_t totalThreads = OccupiedThreads + m_ThreadCount;
		BB_Assert((totalThreads < SystemThreadCount), "Assigning more threads than threads available");

		uint32_t allocSize =
			((m_PoolThreadCount + m_StaticThreadCount) * sizeof(ThreadDesc)) +
			(m_PoolThreadCount * sizeof(uint32_t));

		m_ThreadAlloc.Init(allocSize);

		m_TasksHandled = BBNewArr(m_ThreadAlloc, a_ThreadPoolCount, uint32_t);

		InitializeThreads();
		CreatePoolSystemThread();
	}

	ThreadPool::~ThreadPool()
	{
		m_SystemThread.thdStatus = ThreadStatus::Terminate;

		for (size_t i = 0; i < m_PoolThreadCount; i++) {
			m_PoolThreads[i].thdStatus = ThreadStatus::Terminate;
		}

		for (size_t i = 0; i < m_StaticThreadCount; i++) {
			m_StaticThreads[i].thdStatus = ThreadStatus::Terminate;
		}

		OccupiedThreads -= m_ThreadCount;
		BBFreeArr(m_ThreadAlloc, m_PoolThreads);
		BBFreeArr(m_ThreadAlloc, m_StaticThreads);
		BBFreeArr(m_ThreadAlloc, m_TasksHandled);
	}

	void ThreadPool::AddTask(void (*a_void)(void*), void* a_ThreadFunctionParam, BBTaskHandle* a_Handle)
	{
		*a_Handle = UnprocessedThread;

		TaskDesc task;
		task.tskHandle = a_Handle;
		task.tskFunction = a_void;
		task.tskParam = a_ThreadFunctionParam;
		m_TaskQueue.Add(task);
	}

	bool ThreadPool::IsTaskDone(BBTaskHandle& a_Handle)
	{
		if (a_Handle == UnprocessedThread)
			return false;

		uint32_t taskHandle = (uint32_t)((a_Handle & 0xFFFFFFFF00000000LL) >> 32);
		uint32_t threadHandle = (uint32_t)(a_Handle & 0xFFFFFFFFLL);

		if (m_TasksHandled[threadHandle] >= taskHandle) {
			return true;
		}

		return false;
	}

	void ThreadPool::WaitTillTaskIsDone(BBTaskHandle& a_Handle)
	{
		uint32_t maxWait = MAXINT32;
		uint32_t times = 0;
		while (!IsTaskDone(a_Handle) && times < maxWait) {
			times++;
		}

		if (times >= maxWait) {
			printf("f");
		}
	}

	BBThreadHandle ThreadPool::CreateStaticThread(void(*a_void)(void*), void* a_ThreaFunctionParam)
	{
		if (m_UsedStaticThreads >= m_StaticThreadCount) {
			BB_Log(DEFAULT_LOG_CHANNEL, BBUtility::LogWarningHigh, "No more static threads available");
			return NULL;
		}

		ThreadDesc* thdDesc = &m_StaticThreads[m_UsedStaticThreads];
		thdDesc->thdStatus = ThreadStatus::Working;
		thdDesc->thdId = -1;
		thdDesc->thdTskHandle = nullptr;
		thdDesc->thdFunction = a_void;
		thdDesc->thdParams = a_ThreaFunctionParam;
		thdDesc->thdHandle = CreateThread(
			NULL,
			0,
			ThreadStaticFunction,
			thdDesc,
			0,
			NULL);

		m_UsedStaticThreads++;

		return thdDesc;
	}

	void ThreadPool::DestoryStaticThread(BBThreadHandle a_Handle)
	{
		reinterpret_cast<ThreadDesc*>(a_Handle)->thdStatus = ThreadStatus::Terminate;
	}

	void ThreadPool::InitializeThreads()
	{
		m_StaticThreads = BBNewArr(m_ThreadAlloc, m_StaticThreadCount, ThreadDesc);
		m_PoolThreads = BBNewArr(m_ThreadAlloc, m_PoolThreadCount, ThreadDesc);

		for (int i = 0; i < m_PoolThreadCount; i++) {
			m_TasksHandled[i] = 0;

			ThreadDesc* thdDesc = &m_PoolThreads[i];
			thdDesc->thdStatus = ThreadStatus::Idle;
			thdDesc->thdId = i;
			thdDesc->thdTskHandle = nullptr;
			thdDesc->thdFunction = nullptr;
			thdDesc->thdParams = nullptr;
			thdDesc->thdHandle = CreateThread(
				NULL,
				0,
				ThreadFunction,
				thdDesc,
				0,
				NULL);

			m_Pool.PushFront(thdDesc);
		}
	}

	void ThreadPool::CreatePoolSystemThread()
	{
		m_SystemDesc.sthdQueue = &m_TaskQueue;
		m_SystemDesc.sthdPool = &m_Pool;
		m_SystemDesc.sthdPoolSize = m_PoolThreadCount;
		m_SystemDesc.sthdTasksHandled = m_TasksHandled;
		m_SystemDesc.sthdPoolArray = m_PoolThreads;

		m_SystemThread.thdStatus = ThreadStatus::Working;
		m_SystemThread.thdId = -1;
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