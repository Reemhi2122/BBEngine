#include "FileLoaders/WAVLoader.h"
#include "System/FileHandler.h"
#include "Logger/Logger.h"

namespace BBE {
	
	WAV::WAV()
	{
	}

	WAV::WAV(const char* a_FileName)
	{
		LoadFile(a_FileName);
	}

	void WAV::LoadFile(const char* a_FileName)
	{
		BBSystem::BBFILE file;
		file = BBSystem::OpenFileReadBB(a_FileName);
		
		if (!file) {
			BB_Assert(0, "Unable to open the image file!");
			return;
		}

		size_t BufferSize = BBSystem::GetFileSize(file);
		m_BufferAlloc.Init(BufferSize);
		unsigned char* buffer = BBNewArr(m_BufferAlloc, BufferSize, unsigned char);
		BBSystem::ReadFileBB(file, buffer, BufferSize);

		uint32_t offset = 0;

		m_RiffHeader = reinterpret_cast<RIFF*>(buffer);
		offset += sizeof(RIFF);

		m_FmtHeader = reinterpret_cast<FMT*>(buffer + offset);
		offset += sizeof(FMT);

		m_DataHeader = reinterpret_cast<Data*>(buffer + offset);
		offset += sizeof(Data);

		m_WavBufferAlloc.Init(m_DataHeader->Subchunk2Size);
		m_Data = BBNewArr(m_WavBufferAlloc, m_DataHeader->Subchunk2Size, unsigned char);
		memcpy(m_Data, buffer + offset, m_DataHeader->Subchunk2Size);

		BBSystem::CloseFileBB(file);
	}

}
