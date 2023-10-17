#include "FileLoaders/Sound/WAVLoader.h"
#include "System/FileHandler.h"
#include "Logger/Logger.h"

namespace BBE {
	
	WAV::WAV()
	{
		m_RiffHeader = nullptr;
		m_FmtHeader = nullptr;
		m_DataHeader = nullptr;
		m_Data = nullptr;
	}

	WAV::WAV(const char* a_FileName)
	{
		LoadWav(a_FileName);
	}

	void WAV::LoadWav(const char* a_FilePath)
	{
		BBSystem::BBFILE file;
		file = BBSystem::OpenFileReadBB(a_FilePath);
		
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

		char name[5]{0};
		bool dataRead = false;

		uint8_t whileLimit = 200;
		uint8_t whilecount = 0;

		while (!dataRead && whilecount < whileLimit) {
			memcpy(name, buffer + offset, 4);
			name[4] = 0;

			if (strcmp(name, "fmt ") == 0) {
				m_FmtHeader = reinterpret_cast<FMT*>(buffer + offset);
				offset += sizeof(FMT);
			}

			if (strcmp(name, "data") == 0) {
				m_DataHeader = reinterpret_cast<Data*>(buffer + offset);
				offset += sizeof(Data);

				m_WavBufferAlloc.Init(m_DataHeader->Subchunk2Size);
				m_Data = BBNewArr(m_WavBufferAlloc, m_DataHeader->Subchunk2Size, unsigned char);
				memcpy(m_Data, buffer + offset, m_DataHeader->Subchunk2Size);

				offset += m_DataHeader->Subchunk2Size;
				dataRead = true;
			}
			whilecount++;
		}

		assert(whilecount < whileLimit, "Couldn't find data in WAV file");
		BBSystem::CloseFileBB(file);
	}

	void WAV::WriteWav(const char* a_FilePath)
	{
		BBSystem::BBFILE file;
		file = BBSystem::OpenFileWriteBB(a_FilePath);

		if (!file) {
			BB_Assert(0, "Unable to open create / open write file!");
			return;
		}

		BBSystem::WriteToFileBinary(file, m_RiffHeader, sizeof(RIFF));
		BBSystem::WriteToFileBinary(file, m_FmtHeader, sizeof(FMT));
		BBSystem::WriteToFileBinary(file, m_DataHeader, sizeof(Data));
		BBSystem::WriteToFileBinary(file, m_Data, m_DataHeader->Subchunk2Size);
	}
}
