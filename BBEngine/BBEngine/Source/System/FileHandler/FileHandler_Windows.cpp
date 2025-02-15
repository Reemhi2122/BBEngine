#include "System/FileHandler.h"
#include <Windows.h>
#include <fileapi.h>
#include "Logger/Logger.h"

#include <stdint.h>

namespace BBE {
	namespace BBSystem {
		
		void CreateDirBB(std::string a_Path)
		{
			BOOL isLogCreated = CreateDirectory(a_Path.c_str(), NULL);
		
			if (!isLogCreated && GetLastError() != ERROR_ALREADY_EXISTS) {
				BB_Assert(0, "Couldn't create file!");
			}
		}

		bool FileExistsBB(std::string a_Path)
		{
			DWORD attrib = GetFileAttributes(a_Path.c_str());

			return (attrib != INVALID_FILE_ATTRIBUTES &&
				!(attrib & FILE_ATTRIBUTE_DIRECTORY));
		}

		bool DirectoryExistBB(std::string a_Path)
		{
			DWORD attrib = GetFileAttributes(a_Path.c_str());

			return (attrib != INVALID_FILE_ATTRIBUTES &&
				(attrib & FILE_ATTRIBUTE_DIRECTORY));
		}


		bool GetDirectoryInfo(std::string a_Path, BBDIRECTORY* a_Dir)
		{
			if (!DirectoryExistBB(a_Path))
			{
				a_Dir = nullptr;
				return false;
			}

			a_Path.append("*");
			WIN32_FIND_DATAA data;
			HANDLE previous;
			previous = FindFirstFile(a_Path.c_str(), &data);
			FindNextFile(previous, &data);

			while (FindNextFile(previous, &data))
			{
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					a_Dir->directories[a_Dir->directoryCount].name.append(data.cFileName);
					a_Dir->directoryCount++;
				}
				else
				{
					a_Dir->files[a_Dir->fileCount].name.append(data.cFileName);
					a_Dir->fileCount++;
				}
			}
			return true;
		}

		BBFHANDLE CreateFileBB(std::string a_Path)
		{
			return CreateFile(
				a_Path.c_str(),
				FILE_APPEND_DATA,
				FILE_SHARE_READ,
				NULL,
				CREATE_NEW,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}

		BBFHANDLE OpenFileWriteBB(std::string a_Path) {
			return CreateFile(
				a_Path.c_str(), 
				FILE_GENERIC_WRITE,
				FILE_SHARE_READ, 
				NULL, 
				OPEN_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL);
		}

		BBFHANDLE OpenFileReadBB(std::string a_Path) {
			return CreateFile(
				a_Path.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}

		uint32_t GetFileSize(BBFHANDLE a_Handle)
		{
			LARGE_INTEGER fileSize;
			BOOL res = GetFileSizeEx(a_Handle, &fileSize);
			return static_cast<uint32_t>(fileSize.LowPart);
		}

		void ReadFileBB(BBFHANDLE a_Handle, unsigned char* a_Buffer, uint32_t a_Size) {
			
			DWORD dwBytesRead = 0;
			BOOL result = ReadFile(a_Handle, a_Buffer, a_Size, &dwBytesRead, NULL);

			int test = GetLastError();

			if (!result) {
				BB_LogF(DEFAULT_LOG_CHANNEL, BBUtility::LogWarningHigh,"Cannot read file error code: %d", GetLastError());
				BB_Assert(0, "Cannot read file");
			}

		}

		void ReadFileAtBB(BBFHANDLE a_Handle, unsigned char* a_Buffer, uint32_t a_Size, uint32_t a_Offset)
		{
			SetFilePointer(a_Handle, a_Offset, NULL, FILE_BEGIN);
			ReadFileBB(a_Handle, a_Buffer, a_Size);
			SetFilePointer(a_Handle, 0, NULL, FILE_BEGIN);
		}

		void WriteToFileBB(BBFHANDLE a_File, std::string a_Message)
		{
			WriteFile(a_File, a_Message.c_str(), a_Message.length(), NULL, NULL);
		}

		void WriteToFileBinary(BBFHANDLE a_File, void* buffer, uint32_t a_Size) {
			BOOL res = WriteFile(a_File, buffer, a_Size, NULL, NULL);
			BB_Assert((res == TRUE), "Couldn't write to file!");
		}

		void CloseFileBB(BBFHANDLE a_File)
		{
			CloseHandle(a_File);
		}

	}
}