#include "FileLoaders/Sound/WAVLoader.h"
#include "System/FileHandler.h"
#include "Logger/Logger.h"

namespace BBE {
	
	WAV::WAV()
	{
		m_RiffHeader = nullptr;
		m_FmtData = nullptr;
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

		bool dataRead = false;

		uint8_t whileLimit = 200;
		uint8_t whilecount = 0;

		while (!dataRead && whilecount < whileLimit) {
			WAVHeader header;
			memcpy(&header, buffer + offset, sizeof(WAVHeader));
			offset += sizeof(WAVHeader);

			bool found = false;;
			if (strncmp(header.Type, "fmt ", 4) == 0) {
				memcpy(&m_FmtHeader, &header, sizeof(WAVHeader));

				m_FmtData = reinterpret_cast<FMT*>(buffer + offset);
				offset += sizeof(FMT);
				found = true;
			}

			if (strncmp(header.Type, "data", 4) == 0) {
				memcpy(&m_DataHeader, &header, sizeof(WAVHeader));

				m_WavBufferAlloc.Init(header.ChunkSize);
				m_Data = BBNewArr(m_WavBufferAlloc, header.ChunkSize, unsigned char);
				memcpy(m_Data, buffer + offset, header.ChunkSize);

				offset += header.ChunkSize;
				dataRead = true;
				found = true;
			}

			if (!found) {
				offset += header.ChunkSize;
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
		BBSystem::WriteToFileBinary(file, &m_FmtHeader, sizeof(WAVHeader));
		BBSystem::WriteToFileBinary(file, m_FmtData, sizeof(FMT));
		BBSystem::WriteToFileBinary(file, &m_DataHeader, sizeof(WAVHeader));
		BBSystem::WriteToFileBinary(file, m_Data, m_DataHeader.ChunkSize);
	}
}
