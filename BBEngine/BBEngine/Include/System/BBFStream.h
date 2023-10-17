#pragma once
#include "System/FileHandler.h"
#include "Utility/BBMemory.h"

#include <iostream>

#define FSTREAM_BITFLAG_GOODBIT 1 << 0
#define FSTREAM_BITFLAG_EOFBIT 1 << 1
#define FSTREAM_BITFLAG_FAILBIT 1 << 2
#define FSTREAM_BITFLAG_BADBIT 1 << 3

namespace BBE {
	namespace BBSystem {


		enum class IOF : uint8_t {
			GOODBIT =	1 << 0,
			EOFBIT =	1 << 1,
			FAILBIT =	1 << 2,
			BADBIT =	1 << 3
		};

		class BBFStream {
		public:
			BBFStream();
			BBFStream(const char* a_FilePath);
			~BBFStream();

			void LoadFile(const char* a_FilePath);

			BB_BOOL Get(char& c);
			void Clear(IOF state = IOF::GOODBIT);
			
			bool Good() const noexcept;
			bool Eof() const noexcept;
			bool Fail() const noexcept;
			bool Bad() const noexcept;

		private:
			BBFILE m_File;

			size_t m_BufferPos;
			size_t m_BufferSize;
			unsigned char* m_Buffer;
			uint8_t m_Flags = FSTREAM_BITFLAG_GOODBIT;

			Allocators::ArenaAllocator m_BufferAlloc;
		};

		inline bool BBFStream::Good() const noexcept {
			if ((m_Flags & IOF::GOODBIT) == IOF::GOODBIT) {
				return true;
			}
			return false;
		}

		inline bool BBFStream::Eof() const noexcept
		{
			if ((m_Flags & IOF::EOFBIT) == IOF::EOFBIT) {
				return true;
			}
			return false;
		}

		inline bool BBFStream::Fail() const noexcept
		{
			if ((m_Flags & IOF::FAILBIT) == IOF::FAILBIT) {
				return true;
			}
			return false;
		}

		inline bool BBFStream::Bad() const noexcept
		{
			if ((m_Flags & IOF::BADBIT) == IOF::BADBIT) {
				return true;
			}
			return false;
		}
	}
}