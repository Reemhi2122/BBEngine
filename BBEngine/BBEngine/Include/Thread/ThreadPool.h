#pragma once
namespace BBE {

	typedef void* BBThreadHandle;

	class Thread {

	};

	class ThreadPool
	{
	public:
		ThreadPool(int ThreadCount);
		~ThreadPool();

		BBThreadHandle StartTask(void(*a_ThreadFunction), void* a_ThreaFunctionParam);

	private:
		int m_ThreadCount = 0;

	};

}

