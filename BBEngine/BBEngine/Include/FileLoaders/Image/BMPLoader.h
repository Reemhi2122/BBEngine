#pragma once
#include <stdint.h>
#include <vector>
#include "Allocators/ArenaAllocator.h"
#include "System/FileHandler.h"

namespace BBE {

#pragma pack(push, 1)
	struct BMPFileHeader {
		uint16_t fileType{ 0x4D42 };
		uint32_t fileSize{ 0u };
		uint16_t reserved1{ 0u };
		uint16_t resvered2{ 0u };
		uint32_t offsetData{ 0u };
	};


	struct BMPInfoHeader {
		uint32_t size{ 0u };
		int32_t width{ 0 };
		int32_t height{ 0 };
	
		uint16_t planes{ 1u };
		uint16_t bitCount{ 0u };
		uint32_t compression{ 0u };
		uint32_t sizeImage{ 0u };
		int32_t xPixelPerMeter { 0 };
		int32_t yPixelPerMeter { 0 };
		uint32_t colorsUsed{ 0u };
		uint32_t colorsImportant{ 0u };
	};

	struct BMPColorHeader
	{
		uint32_t red_mask{ 0x00ff0000 };
		uint32_t green_mask{ 0x0000ff00 };
		uint32_t blue_mask{ 0x000000ff };
		uint32_t alpha_mask{ 0xff000000 };
		uint32_t colorSpaceType{ 0x73524742 };
		uint32_t unused[16]{ 0u };
	};
#pragma pack(pop)
	
	class BMP
	{
	public:
		BMP();
		BMP(const char* a_FileName);
		BMP(int32_t a_Width, int32_t a_Height, bool a_Alpha = true);
		~BMP();

		void LoadBMP(const char* a_Name);
		void WriteBMP(const char* a_Name);

		void FillRegion(uint32_t a_X, uint32_t a_Y, uint32_t a_W, uint32_t a_H, uint8_t a_B, uint8_t a_G, uint8_t a_R, uint8_t a_A);

		uint32_t GetHeight() const noexcept;
		uint32_t GetWidth() const noexcept;

		uint8_t* GetBuffer() const;

	private:
		void WriteHeadersAndData(BBSystem::BBFHANDLE& a_FileHandle);
		void WriteHeaders(BBSystem::BBFHANDLE& a_FileHandle);

		void CheckColorHeader(BMPColorHeader& a_ColorHeader);
		uint32_t MakeStrideAligned(uint32_t align_stride);

		uint32_t m_RowStride{ 0 };

		BMPFileHeader m_FileHeader;
		BMPInfoHeader m_InfoHeader;
		BMPColorHeader m_ColorHeader;
		uint8_t* m_Data;

		Allocators::ArenaAllocator m_BMPAlloc;
		Allocators::ArenaAllocator m_BMPPaddingAlloc;
		Allocators::ArenaAllocator m_BMPBufferAlloc;
	};

	inline uint32_t BMP::GetHeight() const noexcept 
	{
		return m_InfoHeader.height;
	}

	inline uint32_t BMP::GetWidth() const noexcept
	{
		return m_InfoHeader.width;
	}

	inline uint8_t* BMP::GetBuffer() const
	{
		return m_Data;
	}
}