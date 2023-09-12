#pragma once
#include "BBRenderer.h"
#include "BBWindow.h"
#include "Timer/BBTimer.h"
#include "Vector3.h"

class BBEngine {
public:
	BBEngine();
	int StartBBEngine();
	void TestCode();

private:
	void Update();

private:
	BBWindow m_Window;
	BBTimer m_Timer;
	BBWinRenderer m_BBWinRenderer;
};