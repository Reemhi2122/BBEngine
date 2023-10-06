#pragma once
#include <stdint.h>
#include <vector>

namespace BBE {

#pragma pack(push, 1)
	struct BMPFileHeader {
		uint16_t fileType{ 0x4D42 };
		uint32_t fileSize{ 0 };
		uint16_t reserved1{ 0 };
		uint16_t resvered2{ 0 };
		uint32_t offsetData{ 0 };
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
		uint32_t colorsUsed{ 0 };
		uint32_t colorsImportant{ 0 };
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
		BMP() = default;
		BMP(const char* a_FileName);
		BMP(int32_t a_Width, int32_t a_Height, bool a_Alpha = true);
		~BMP() = default;

		void LoadBMP(const char* a_Name);
		void WriteBMP(const char* a_Name);

	private:
		void CheckColorHeader(BMPColorHeader& a_ColorHeader);

		BMPFileHeader m_FileHeader;
		BMPInfoHeader m_InfoHeader;
		BMPColorHeader m_ColorHeader;
		std::vector<uint8_t> data;


	};

}