#include "FileLoaders/BMPLoader.h"
#include "Logger/Logger.h"
#include "Utility/BBMemory.h"
#include <algorithm>

namespace BBE {
	BMP::BMP()
	{
		m_Data = nullptr;
	}

	BMP::BMP(const char* a_FileName)
	{
		m_Data = nullptr;
		LoadBMP(a_FileName);
	}
	
	BMP::BMP(int32_t a_Width, int32_t a_Height, bool a_HasAlpha)
	{
		BB_Assert((a_Width > 0 || a_Height > 0), "The image width and height must be positive");

		m_InfoHeader.width = a_Width;
		m_InfoHeader.height = a_Height;

		m_BMPBufferAlloc.Init(4 * MBSize);

		if (a_HasAlpha) {
			m_InfoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			m_FileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			
			m_InfoHeader.bitCount = 32;
			m_InfoHeader.compression = 3;
			m_RowStride = a_Width * 4;
			m_Data = BBNewArr(m_BMPBufferAlloc, (m_RowStride * a_Height), uint8_t);
			m_FileHeader.fileSize = m_FileHeader.offsetData + (m_RowStride * a_Height);
		}
		else {
			m_InfoHeader.size = sizeof(BMPInfoHeader);
			m_FileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
			
			m_InfoHeader.bitCount = 24;
			m_InfoHeader.compression = 0;
			m_RowStride = a_Width * 3;
			m_Data = BBNewArr(m_BMPBufferAlloc, (m_RowStride * a_Height), uint8_t);
			
			uint32_t newStride = MakeStrideAligned(4);
			m_FileHeader.fileSize = m_FileHeader.offsetData + (m_RowStride * a_Height) + m_InfoHeader.height * (newStride - m_RowStride);
		}
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

		if (!file) {
			BBSystem::CloseFileBB(file);
			BB_Assert(0, "Unable to open the image file!");
			return;
		}

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
		m_Data = BBNewArr(m_BMPAlloc, dataSize, uint8_t);

		if (m_InfoHeader.width % 4 == 0) {
			memcpy(m_Data, buffer + offset, dataSize);
			m_FileHeader.fileSize += (dataSize * sizeof(uint8_t));
		}
		else {
			m_RowStride = m_InfoHeader.width * m_InfoHeader.bitCount / 8;
			uint32_t newStride = MakeStrideAligned(4);
			uint32_t strideOffset = newStride - m_RowStride;
			m_BMPPaddingAlloc.Init(strideOffset);

			uint32_t* paddingRow = BBNewArr(m_BMPPaddingAlloc, strideOffset, uint32_t);
			uint8_t* ptr = reinterpret_cast<uint8_t*>(m_Data);

			for (int y = 0; y < m_InfoHeader.height; ++y) {
				memcpy(ptr, buffer + offset, m_RowStride);
				offset += m_RowStride;
				memcpy(paddingRow, buffer + offset, strideOffset);
				offset += strideOffset;
				ptr += m_RowStride;
			}

			m_FileHeader.fileSize += dataSize + m_InfoHeader.height * (newStride - m_RowStride);
			m_BMPPaddingAlloc.Clear();
		}

		BBSystem::CloseFileBB(file);
	}
	
	void BMP::WriteBMP(const char* a_Name)
	{
		BBSystem::BBFILE file;
		file = BBSystem::OpenFileWriteBB(a_Name);

		if (!file) {
			BBSystem::CloseFileBB(file);
			BB_Assert(0, "Unable to open the image file!");
			return;
		}

		if (m_InfoHeader.bitCount == 32) {
			WriteHeadersAndData(file);
		}
		else if (m_InfoHeader.bitCount == 24) {
			if (m_InfoHeader.width % 4 == 0) {
				WriteHeadersAndData(file);
			}
			else {
				uint32_t newStride = MakeStrideAligned(4);
				uint32_t strideOffset = newStride - m_RowStride;
				uint32_t* paddingRow = BBNewArr(m_BMPPaddingAlloc, strideOffset, uint32_t);

				int offset = 0;
				WriteHeaders(file);
				for (int y = 0; y < m_InfoHeader.height; y++) {
					BBSystem::WriteToFileBinary(file, Pointer::Add(m_Data, offset), m_RowStride);
					offset += m_RowStride;
					BBSystem::WriteToFileBinary(file, paddingRow, strideOffset);
				}
			}
		}
		else {
			BB_Assert(0, "The program can treat only 24 or 32 bits per pixel!");
		}

		BBSystem::CloseFileBB(file);
	}

	void BMP::FillRegion(uint32_t a_X, uint32_t a_Y, uint32_t a_W, uint32_t a_H, uint8_t a_B, uint8_t a_G, uint8_t a_R, uint8_t a_A)
	{
		if (a_X + a_W > (uint32_t)m_InfoHeader.width || a_Y + a_H > (uint32_t)m_InfoHeader.height ) {
			BB_Assert(0, "Trying to fill region outside of BMP!");
		}

		uint32_t channels = m_InfoHeader.bitCount / 8;
		for (uint32_t y = a_Y; y < (a_Y + a_H); y++) {
			for (uint32_t x = a_X; x < (a_X + a_W); x++)
			{
				m_Data[channels * (y * m_InfoHeader.width + x) + 0] = a_B;
				m_Data[channels * (y * m_InfoHeader.width + x) + 1] = a_G;
				m_Data[channels * (y * m_InfoHeader.width + x) + 2] = a_R;
				if (channels == 4) {
					m_Data[channels * (y * m_InfoHeader.width + x) + 3] = a_A;
				}
			}
		}
	}

	void BMP::ApplyBlur()
	{
		float Gausian3x3[9]{
			1.f, 2.f, 1.f,
			2.f, 4.f, 2.f,
			1.f, 2.f, 1.f
		};

		uint32_t width = m_InfoHeader.width;
		uint32_t height = m_InfoHeader.height;

		uint32_t channels = m_InfoHeader.bitCount / 8;

		for (int y = 0; y < height; y ++) {
			for (int x = 0; x < width; x++) {
				
				int matrixIndex = 0;
				uint8_t RAccumulator = 0;
				uint8_t GAccumulator = 0;
				uint8_t BAccumulator = 0;

				uint32_t accum = 0;

				for (int r = -1; r < 2; r++)
				{
					for (int c = -1; c < 2; c++) {
						int y0 = std::clamp((y+r), 0, (int)height);
						int x0 = std::clamp((x+c), 0, (int)width);

						uint32_t PixelIndex = channels * (y0 * m_InfoHeader.width + x0);

						RAccumulator += m_Data[PixelIndex + 0] * Gausian3x3[matrixIndex] * 0.0625;
						GAccumulator += m_Data[PixelIndex + 1] * Gausian3x3[matrixIndex] * 0.0625;
						BAccumulator += m_Data[PixelIndex + 2] * Gausian3x3[matrixIndex] * 0.0625;
						matrixIndex++;
					}
				}

				uint32_t PixelIndex = channels * (y * m_InfoHeader.width + x);
				m_Data[PixelIndex] = accum;
				m_Data[PixelIndex + 0] = RAccumulator;
				m_Data[PixelIndex + 1] = GAccumulator;
				m_Data[PixelIndex + 2] = BAccumulator;
			}
		}

	}

	void BMP::WriteHeaders(BBSystem::BBFILE& a_FileHandle)
	{
		BBSystem::WriteToFileBinary(a_FileHandle, &m_FileHeader, sizeof(BMPFileHeader));
		BBSystem::WriteToFileBinary(a_FileHandle, &m_InfoHeader, sizeof(BMPInfoHeader));
		if(m_InfoHeader.bitCount == 32) {
			BBSystem::WriteToFileBinary(a_FileHandle, &m_ColorHeader, sizeof(BMPColorHeader));
		}
	}

	void BMP::WriteHeadersAndData(BBSystem::BBFILE& a_FileHandle)
	{
		WriteHeaders(a_FileHandle);
		uint32_t size = (m_InfoHeader.width * m_InfoHeader.height) * (m_InfoHeader.bitCount / 8);
		BBSystem::WriteToFileBinary(a_FileHandle, m_Data, size);
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