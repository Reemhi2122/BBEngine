#pragma once
#include "System/FileHandler.h"

namespace BBE {
	namespace BBSystem {

		class BBFStream {
		public:
			BBFStream(const char* a_FilePath);

		private:
			BBFile m_File;

			uint32_t m_BufferStart;
			uint32_t m_BufferPos;
			char* buffer;
		};

	}
}