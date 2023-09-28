#pragma once
#include "Windows.h"

namespace BBE {

		enum class ThreadState {
			Idle,
			Working,
			Blocked
		};

		class Thread
		{
		public:
			Thread();
			~Thread();

		private:

		};

}