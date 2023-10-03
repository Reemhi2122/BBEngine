#pragma once
#include <cstdint>

#include "Utility/BBMemory.h" 
#include "Containers/Pool.h"
#include "Containers/Queue.h"

namespace BBE {

	typedef void* BBTaskHandle;
	typedef void* BBThreadHandle;

	enum class ThreadStatus {
		Idle,
		Working,
		Terminate
	};

	enum class TaskStatus {
		Queue,
		Process,
		Done
	};

	struct ThreadTaskDesc {
		TaskStatus		tskStatus;
		void*			tskFunction;
		void*			tskParam;
	};

	class ThreadPool
	{
	public:
		ThreadPool() = default;
		ThreadPool(const uint8_t& AmountOfStaticThreads);
		~ThreadPool();

		BBTaskHandle AddTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam = NULL);
		uint8_t GetNumberOfStaticThreads() const;
		uint8_t GetNumberOfPoolThreads() const;

	private:
		void InitializePoolThreads(const uint8_t& a_Count);

		uint8_t m_ThreadCount = 0u;
		uint8_t m_PoolThreadCount = 0u;
		uint8_t m_StaticThreadCount = 0u;

		Queue<ThreadTaskDesc> TaskQueue;
		Pool<HANDLE> m_Pool;

		Allocators::StackAllocator m_ThreadAlloc;

		static uint8_t OccupiedThreads;
	};

	uint8_t ThreadPool::OccupiedThreads = 0;

	inline uint8_t ThreadPool::GetNumberOfStaticThreads() const
	{
		return m_StaticThreadCount;
	}

	inline uint8_t ThreadPool::GetNumberOfPoolThreads() const
	{
		return m_PoolThreadCount;
	}
}

