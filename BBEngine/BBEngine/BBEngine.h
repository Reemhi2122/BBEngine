#pragma once
#include "BBRenderer.h"
#include "BBWindow.h"
#include "Utility/BBTimer.h"
#include "Vector3.h"

#include "Thread/ThreadPool.h"

class BBEngine {
public:
	BBEngine();
	~BBEngine();

	int StartBBEngine();
	void TestCode();

private:
	void Update();

private:
	BBWindow m_Window;
	BBE::BBTimer m_Timer;
	BBWinRenderer m_BBWinRenderer;

	BBE::Allocators::StackAllocator m_StackAlloc;

	BBE::ThreadPool* m_ThreadPool;
};