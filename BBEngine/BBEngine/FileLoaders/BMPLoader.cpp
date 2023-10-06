#include "FileLoaders/BMPLoader.h"
#include "System/FileHandler.h"
#include "Logger/Logger.h"
#include "Utility/BBMemory.h"

namespace BBE {
	
	BMP::BMP(const char* a_FileName)
	{

	}
	
	BMP::BMP(int32_t a_Width, int32_t a_Height, bool a_Alpha)
	{

	}
	
	void BMP::LoadBMP(const char* a_Name)
	{
		BBSystem::BBFILE file;
		file = BBSystem::OpenFileReadBB(a_Name);


		if (file) {
			char buffer[BUFFERSIZE] = "";
			BBSystem::ReadFileBB(file, buffer);

			uint32_t offset = 0;
			m_FileHeader = *reinterpret_cast<BMPFileHeader*>(buffer);
			offset += sizeof(BMPFileHeader);

			BB_Assert((m_FileHeader.fileType == 0x4D42), "Loaded BMP file is not of correct file type");

			m_InfoHeader = *reinterpret_cast<BMPInfoHeader*>(buffer + offset);
			offset += sizeof(BMPInfoHeader);

			if (m_InfoHeader.bitCount == 32) {
				if (m_InfoHeader.size >= sizeof(BMPInfoHeader) + sizeof(BMPColorHeader)) {
					m_ColorHeader = *reinterpret_cast<BMPColorHeader*>(buffer + offset);
					CheckColorHeader(m_ColorHeader);
				}
				else {
					BB_Log(DEFAULT_LOG_CHANNEL, BBUtility::LogWarningHigh, "The file %s does not contain bit mask information", a_Name);
					BB_Assert(0, "Unrecognized file!");
				}
			}

			//Not sure about the jumping yet
			offset += m_FileHeader.offsetData;


			if (m_InfoHeader.bitCount == 32) {
				m_InfoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
				m_FileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			}
			else {
				m_InfoHeader.size = sizeof(BMPInfoHeader);
				m_FileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
			}
			m_FileHeader.fileSize = m_FileHeader.offsetData;

			BB_Assert((m_InfoHeader.height > 0), "This BMP converter only supports images with the origin in the bottom left corner.");

			data.resize(m_InfoHeader.width * m_InfoHeader.height * m_InfoHeader.bitCount / 8);

			if (m_InfoHeader.width % 4 == 0) {

			}

		} else {
			
		}

		BBSystem::CloseFileBB(file);
	}
	
	void BMP::WriteBMP(const char* a_Name)
	{

	}

	void BMP::CheckColorHeader(BMPColorHeader& a_ColorHeader)
	{
	}
}