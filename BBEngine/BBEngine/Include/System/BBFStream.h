#pragma once
#include "System/FileHandler.h"
#include "Utility/BBMemory.h"

#define FSTREAM_BITFLAG_GOODBIT 1 << 0
#define FSTREAM_BITFLAG_EOFBIT 1 << 1
#define FSTREAM_BITFLAG_FAILBIT 1 << 2
#define FSTREAM_BITFLAG_BADBIT 1 << 3

namespace BBE {
	namespace BBSystem {

		class BBFStream {
		public:
			BBFStream();
			BBFStream(const char* a_FilePath);
			~BBFStream();

			void LoadFile(const char* a_FilePath);

			BB_BOOL Get(char& c);
			
			bool Good();

		private:
			BBFILE m_File;

			size_t m_BufferPos;
			size_t m_BufferSize;
			unsigned char* m_Buffer;
			uint8_t m_Flags = FSTREAM_BITFLAG_GOODBIT;

			Allocators::ArenaAllocator m_BufferAlloc;
		};

	}
}