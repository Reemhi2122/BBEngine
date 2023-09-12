#pragma once
#include "Allocator.h"

namespace BBEngine {
	namespace Allocators {

		struct Arena {
			unsigned char* allocBuf;
			unsigned int bufLeng;
			unsigned int prevOffset;
			unsigned int currOffset;
		};

		class ArenaAllocator : Allocator {
			
		public:
			ArenaAllocator();
			~ArenaAllocator();



		private:

		};

	}
}