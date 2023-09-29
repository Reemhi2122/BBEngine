#pragma once
#include <cstdint>

#include "Utility/BBMemory.h" 

namespace BBE {

	typedef void* BBThreadHandle;

	class ThreadPool
	{
	public:
		ThreadPool(const uint8_t& AmountOfStaticThreads);
		~ThreadPool();

		BBThreadHandle AddTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam = NULL);
		uint8_t GetNumberOfStaticThreads() const;
		uint8_t GetNumberOfPoolThreads() const;


	private:
		uint8_t m_ThreadCount = 0u;
		uint8_t m_PoolThreadCount = 0u;
		uint8_t m_StaticThreadCount = 0u;

		struct Thread* m_PoolThreads;
		struct Thread* m_StaticThreads;
		Allocators::StackAllocator m_ThreadAlloc;
	};

}

