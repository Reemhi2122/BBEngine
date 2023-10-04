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

	struct TaskDesc {
		TaskStatus		tskStatus;
		void			(*tskFunction)(void*);
		void*			tskParam;
	};

	struct ThreadDesc {
		ThreadStatus	thdStatus;
		BBThreadHandle	thdHandle;
		void			(*thdFunction)(void*);
		void*			thdParams;
	};

	struct SystemThreadDesc {
		Queue<TaskDesc>* sthdQueue;
		Pool<ThreadDesc>* sthdPool;
	};

	class ThreadPool
	{
	public:
		ThreadPool() = default;
		ThreadPool(const uint8_t& AmountOfStaticThreads);
		~ThreadPool();

		BBTaskHandle AddTask(void (*a_void)(void*), void* a_ThreaFunctionParam = NULL);
		uint8_t GetNumberOfStaticThreads() const;
		uint8_t GetNumberOfPoolThreads() const;

		static uint8_t OccupiedThreads;

	private:
		void InitializePoolThreads(const uint8_t& a_Count);
		void CreatePoolSystemThread();

		uint8_t m_ThreadCount = 0u;
		uint8_t m_PoolThreadCount = 0u;
		uint8_t m_StaticThreadCount = 0u;

		Queue<TaskDesc> m_TaskQueue;
		Pool<ThreadDesc> m_Pool = Pool<ThreadDesc>(32);

		SystemThreadDesc m_SystemDesc;
		ThreadDesc m_SystemThread;

		ThreadDesc* m_PoolThreads;

		Allocators::StackAllocator m_ThreadAlloc;
	};


	inline uint8_t ThreadPool::GetNumberOfStaticThreads() const
	{
		return m_StaticThreadCount;
	}

	inline uint8_t ThreadPool::GetNumberOfPoolThreads() const
	{
		return m_PoolThreadCount;
	}
}
