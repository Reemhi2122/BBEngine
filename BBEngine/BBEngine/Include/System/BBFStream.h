#pragma once
#include "System/FileHandler.h"
#include "Utility/BBMemory.h"

#include <iostream>

namespace BBE {
	namespace BBSystem {

		namespace FSF {
			using FSFlag = uint8_t;

			static constexpr int GOODBIT = 1 << 0;
			static constexpr int EOFBIT = 1 << 1;
			static constexpr int FAILBIT = 1 << 2;
			static constexpr int BADBIT = 1 << 3;
		}

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
			BB_BOOL SeekPos(uint8_t a_Pos);
			uint8_t GetPos();
			void Clear(FSF::FSFlag flag = FSF::GOODBIT);


			
			bool Good() const noexcept;
			bool Eof() const noexcept;
			bool Fail() const noexcept;
			bool Bad() const noexcept;

		private:
			BBFILE m_File;

			size_t m_BufferPos;
			size_t m_BufferSize;
			unsigned char* m_Buffer;
			uint8_t m_Flags = FSF::BADBIT;

			Allocators::ArenaAllocator m_BufferAlloc;
		};

		inline bool BBFStream::Good() const noexcept {
			if ((m_Flags & FSF::GOODBIT) == FSF::GOODBIT) {
				return true;
			}
			return false;
		}

		inline bool BBFStream::Eof() const noexcept
		{
			if ((m_Flags & FSF::EOFBIT) == FSF::EOFBIT) {
				return true;
			}
			return false;
		}

		inline bool BBFStream::Fail() const noexcept
		{
			if ((m_Flags & FSF::FAILBIT) == FSF::FAILBIT) {
				return true;
			}
			return false;
		}

		inline bool BBFStream::Bad() const noexcept
		{
			if ((m_Flags & FSF::BADBIT) == FSF::BADBIT) {
				return true;
			}
			return false;
		}

		inline uint8_t BBFStream::GetPos() {
			return m_BufferPos;
		}
	}
}