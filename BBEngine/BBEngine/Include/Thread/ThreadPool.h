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
		Waiting,
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
		TaskDesc*		thdTskHandle;
		BBThreadHandle	thdHandle;
		void			(*thdFunction)(void*);
		void*			thdParams;
	};

	struct SystemThreadDesc {
		Queue<TaskDesc>*	sthdQueue;
		Pool<ThreadDesc>*	sthdPool;
		ThreadDesc*			sthdPoolArray;
		uint8_t				sthdPoolSize;
	};

	class ThreadPool
	{
	public:
		ThreadPool() = default;
		ThreadPool(const uint8_t& AmountOfStaticThreads, const uint8_t& a_StaticThreadCount = 0);
		~ThreadPool();

		BBTaskHandle AddTask(void (*a_void)(void*), void* a_ThreaFunctionParam = NULL);
		BBThreadHandle CreateStaticThread(void (*a_void)(void*), void* a_ThreaFunctionParam = NULL);
		void DestoryStaticThread(BBThreadHandle a_Handle);

		uint8_t GetThreadCount() const noexcept;
		uint8_t GetPoolThreadCount() const noexcept;
		uint8_t GetStaticThreadCount() const noexcept;

		static uint8_t OccupiedThreads;

	private:
		void InitializeThreads();
		void CreatePoolSystemThread();

		uint8_t m_ThreadCount = 0u;
		uint8_t m_PoolThreadCount = 0u;
		uint8_t m_StaticThreadCount = 0u;

		uint8_t m_UsedStaticThreads = 0u;

		Queue<TaskDesc> m_TaskQueue;
		Pool<ThreadDesc> m_Pool = Pool<ThreadDesc>(32u);

		SystemThreadDesc m_SystemDesc;
		ThreadDesc m_SystemThread;
		
		ThreadDesc* m_PoolThreads;
		ThreadDesc* m_StaticThreads;

		Allocators::StackAllocator m_ThreadAlloc;
	};

	inline uint8_t BBE::ThreadPool::GetThreadCount() const noexcept
	{
		return m_ThreadCount;
	}

	inline uint8_t ThreadPool::GetPoolThreadCount() const noexcept
	{
		return m_PoolThreadCount;
	}

	inline uint8_t ThreadPool::GetStaticThreadCount() const noexcept
	{
		return m_StaticThreadCount;
	}
}
