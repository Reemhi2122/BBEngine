#pragma once
#include <cstdint>

#include "Utility/BBMemory.h" 

namespace BBE {

	typedef void* BBThreadHandle;

	enum class ThreadStatus {
		Idle,
		Working,
		Terminate
	};

	class ThreadPool
	{
	public:
		ThreadPool() = default;
		ThreadPool(const uint8_t& AmountOfStaticThreads);
		~ThreadPool();

		BBThreadHandle AddTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam = NULL);
		uint8_t GetNumberOfStaticThreads() const;
		uint8_t GetNumberOfPoolThreads() const;

	private:
		void InitializeThreads(const uint8_t& a_Count);

		uint8_t m_ThreadCount = 0u;
		uint8_t m_PoolThreadCount = 0u;
		uint8_t m_StaticThreadCount = 0u;

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

