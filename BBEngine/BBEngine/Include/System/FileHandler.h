#pragma once
#include <string>

#define BUFFERSIZE  8 * 1024 * 1024
#define BB_TRUE 0
#define BB_FALSE 1

namespace BBE {
	namespace BBSystem {
		
		using DWORD = unsigned long;
		using BBFHANDLE = void*;
		using BB_BOOL = int;


		struct BBFILEINFO
		{
			std::string name;
		};

		struct BBDIRECTORYINFO
		{
			std::string name;
		};

		struct BBDIRECTORY
		{
			uint32_t fileCount = 0;
			uint32_t directoryCount = 0;
			BBDIRECTORYINFO directories[100];
			BBFILEINFO files[100];
		};

		void CreateDirBB(std::string a_Path);
		bool FileExistsBB(std::string a_Path);
		bool DirectoryExistBB(std::string a_Path);

		bool GetDirectoryInfo(std::string a_Path, BBDIRECTORY* a_Dir);
			
		BBFHANDLE CreateFileBB(std::string a_Path);
		BBFHANDLE OpenFileWriteBB(std::string a_Path);
		BBFHANDLE OpenFileReadBB(std::string a_Path);

		uint32_t GetFileSize(BBFHANDLE a_Handle);

		void ReadFileBB(BBFHANDLE a_Handle, unsigned char* a_Buffer, uint32_t a_Size = BUFFERSIZE);
		void ReadFileAtBB(BBFHANDLE a_Handle, unsigned char* a_Buffer, uint32_t a_Size = BUFFERSIZE, uint32_t a_Offset = 0);

		void WriteToFileBinary(BBFHANDLE a_File, void* buffer, uint32_t a_Size);
		void WriteToFileBB(BBFHANDLE a_File, std::string a_Message);

		void CloseFileBB(BBFHANDLE a_File);
	}
}