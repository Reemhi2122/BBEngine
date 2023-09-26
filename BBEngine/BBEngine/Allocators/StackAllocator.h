#pragma once
#include "allocator.h"

namespace BBE {
	namespace Allocators {

		struct Stack {
			void* buf;
			size_t bufSize;
			size_t curOffset;
			size_t setpoint;
		};

		struct StackHeader {
			size_t size;
			size_t prevOffset;
		};

		class StackAllocator : public Allocator {
			
		public:
			StackAllocator();
			~StackAllocator();

			StackAllocator& operator=(const StackAllocator& rhs) = delete;

			void Init(size_t a_Size, const size_t& a_Allignment = DEFAULT_ALIGNMENT, const size_t& a_ChunkSize = 0) override;
			void* Alloc(size_t a_Size, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void* Realloc(void* a_Ptr, size_t a_OldSize, size_t a_NewSize, const size_t& a_Align = DEFAULT_ALIGNMENT) override;
			void Free(void* a_Ptr) override;
			void Clear();

			int SetPoint();
			void ReturnToPoint();

		private:
			Stack m_Stack;
		};

	}
}