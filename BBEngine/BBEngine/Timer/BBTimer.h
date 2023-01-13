#pragma once
#include <chrono>

class BBTimer {
public:
	BBTimer();
	float Stamp();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point m_Last;
};