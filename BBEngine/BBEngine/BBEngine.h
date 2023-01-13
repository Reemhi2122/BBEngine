#pragma once
#include "../BBRenderer/BBRenderer.h"
#include "../BBRenderer/Window/BBWindow.h"
#include "Timer/BBTimer.h"

class BBEngine {
public:
	BBEngine();
	int StartBBEngine();

private:
	void Update();

private:
	BBWindow m_Window;
	BBTimer m_Timer;
	BBWinRenderer m_BBWinRen;

};