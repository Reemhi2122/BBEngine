#include "FileLoaders/WAVLoader.h"
#include "System/FileHandler.h"
#include "Logger/Logger.h"

namespace BBE {
	
	WAV::WAV()
	{
		
	}

	WAV::WAV(const char* a_FileName)
	{

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
		unsigned char* buffer = BBNewArr(m_BufferAlloc, BufferSize, unsigned char);
		BBSystem::ReadFileBB(file, buffer, BufferSize);

	}

}
