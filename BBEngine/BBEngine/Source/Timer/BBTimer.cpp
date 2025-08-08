#include "Utility/BBTimer.h"

namespace BBE {

	BBTimer::BBTimer()
	{
		m_Last = std::chrono::steady_clock::now();
	}

	float BBTimer::Stamp()
	{
		const auto old = m_Last;
		m_Last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = m_Last - old;
		return frameTime.count();
	}

	float BBTimer::Peek() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_Last).count();
	}
}