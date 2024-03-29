#pragma once
#include <string>

#define BUFFERSIZE  8 * 1024 * 1024
#define BB_TRUE 0
#define BB_FALSE 1

namespace BBE {
	namespace BBSystem {
		
		using DWORD = unsigned long;
		using BBFILE = void*;

		using BB_BOOL = int;

		void CreateDirBB(std::string path);
		bool FileExistsBB(std::string path);

		BBFILE CreateFileBB(std::string a_Path);
		BBFILE OpenFileWriteBB(std::string a_Path);
		BBFILE OpenFileReadBB(std::string a_Path);

		uint32_t GetFileSize(BBFILE a_Handle);

		void ReadFileBB(BBFILE a_Handle, unsigned char* a_Buffer, uint32_t a_Size = BUFFERSIZE);
		void ReadFileAtBB(BBFILE a_Handle, unsigned char* a_Buffer, uint32_t a_Size = BUFFERSIZE, uint32_t a_Offset = 0);

		void WriteToFileBinary(BBFILE a_File, void* buffer, uint32_t a_Size);
		void WriteToFileBB(BBFILE a_File, std::string a_Message);

		void CloseFileBB(BBFILE a_File);
	}
}