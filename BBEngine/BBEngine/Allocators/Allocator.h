#pragma once

namespace BBEngine {
	namespace Allocators{
		
		class Allocator {

		public:
			Allocator() = default;
			~Allocator() = default;

			virtual void init() = 0;

			virtual void alloc() = 0;
			virtual void realloc() = 0;
			virtual void free() = 0;

		private:
			

		};
	}
}