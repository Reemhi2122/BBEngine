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

	BMP::~BMP()
	{
		m_BMPAlloc.Clear();
		m_BMPPaddingAlloc.Clear();
	}
	
	void BMP::LoadBMP(const char* a_Name)
	{
		BBSystem::BBFILE file;
		file = BBSystem::OpenFileReadBB(a_Name);


		if (file) {
			m_BMPBufferAlloc.Init(4*MBSize);
			char* buffer = BBAlloc(m_BMPBufferAlloc, 4 * MBSize, char*);
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
			offset = m_FileHeader.offsetData;

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

			int dataSize = m_InfoHeader.width * m_InfoHeader.height * m_InfoHeader.bitCount / 8;
			m_BMPAlloc.Init(dataSize);
			data = BBNewArr(m_BMPAlloc, dataSize, uint8_t);

			if (m_InfoHeader.width % 4 == 0) {
				memcpy(data, buffer + offset, dataSize);
				m_FileHeader.fileSize += (dataSize * sizeof(uint8_t));
			}
			else {
				m_RowStride = m_InfoHeader.width * m_InfoHeader.bitCount / 8;
				uint32_t newStride = MakeStrideAligned(4);
				uint32_t strideOffset = newStride - m_RowStride;
				m_BMPPaddingAlloc.Init(strideOffset);

				uint32_t* paddingRow = BBNewArr(m_BMPPaddingAlloc, strideOffset, uint32_t);
				uint8_t* ptr = reinterpret_cast<uint8_t*>(data);

				for (int y = 0; y < m_InfoHeader.height; ++y) {
					memcpy(ptr, buffer + offset, m_RowStride);
					offset += m_RowStride;
					memcpy(paddingRow, buffer + offset, strideOffset);
					offset += strideOffset;
					ptr += m_RowStride;
				}
				
				m_FileHeader.fileSize += dataSize + m_InfoHeader.height * (newStride - m_RowStride);
			}

		} else {
			BBSystem::CloseFileBB(file);
			BB_Assert(0, "Unable to open the image file!");
		}

		BBSystem::CloseFileBB(file);
	}
	
	void BMP::WriteBMP(const char* a_Name)
	{

	}

	void BMP::CheckColorHeader(BMPColorHeader& a_ColorHeader)
	{
		BMPColorHeader expectedColorHeader;
		if (expectedColorHeader.red_mask != a_ColorHeader.red_mask ||
			expectedColorHeader.blue_mask != a_ColorHeader.blue_mask ||
			expectedColorHeader.green_mask != a_ColorHeader.green_mask ||
			expectedColorHeader.alpha_mask != a_ColorHeader.alpha_mask ) {
			BB_Assert(0, "Loading file with the unexpected color mask format!");
		}
		if (expectedColorHeader.colorSpaceType != a_ColorHeader.colorSpaceType) {
			BB_Assert(0, "Loading file with the unexpected color mask format!");
		}
	}

	uint32_t BMP::MakeStrideAligned(uint32_t a_AlignStride)
	{
		uint32_t newStride = m_RowStride;
		while (newStride % a_AlignStride != 0) {
			newStride++;
		}
		return newStride;
	}
}