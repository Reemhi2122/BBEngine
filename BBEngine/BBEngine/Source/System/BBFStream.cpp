#include "System/BBFStream.h"
#include "Logger/Logger.h"

namespace BBE {
	namespace BBSystem {
		
		BBFStream::BBFStream()
		{
			m_BufferPos = 0;
			m_BufferSize = 0;
			m_Buffer = nullptr;
			m_File = NULL;
		}

		BBFStream::BBFStream(const char* a_FilePath) 
			: BBFStream() {
			LoadFile(a_FilePath);
		}

		BBFStream::~BBFStream()
		{
			m_BufferPos = 0;
			m_Buffer = nullptr;
			m_File = NULL;
		}

		void BBFStream::LoadFile(const char* a_FilePath)
		{
			m_File = OpenFileReadBB(a_FilePath);

			if (!m_File) {
				BB_Assert(0, "Unable to open the image file!");
				return;
			}

			m_BufferSize = GetFileSize(m_File);
			m_BufferAlloc.Init(m_BufferSize);
			m_Buffer = BBNewArr(m_BufferAlloc, m_BufferSize, unsigned char);
			ReadFileBB(m_File, m_Buffer, m_BufferSize);

			Clear();
		}

		BB_BOOL BBFStream::Get(char& c)
		{
			if ((m_BufferPos + 1) > m_BufferSize) {
				m_Flags = FSF::EOFBIT;
				return BB_FALSE;
			}

			c = m_Buffer[m_BufferPos++];
			return BB_TRUE;
		}

		BB_BOOL BBFStream::SeekPos(size_t a_Pos)
		{
			if (a_Pos < 0 || a_Pos > m_BufferSize) {
				return BB_FALSE;
			}

			m_BufferPos = a_Pos;
			return BB_TRUE;
		}

		void BBFStream::Clear(FSF::FSFlag flag)
		{
			if (m_Buffer == nullptr) {
				m_Flags = FSF::BADBIT;
			}
			m_Flags = flag;
		}
	}
}