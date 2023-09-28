#pragma once
#include <chrono>

namespace BBE {
	class BBTimer {
	public:
		BBTimer();
		float Stamp();
		float Peek() const;

	private:
		std::chrono::steady_clock::time_point m_Last;
	};
}