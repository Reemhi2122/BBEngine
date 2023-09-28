#include "System/FileHandler.h"
#include <Windows.h>
#include <fileapi.h>
#include "Logger/Logger.h"

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

			return false;
		}

		BBFILE CreateFileBB(std::string a_Path)
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

		BBFILE OpenFileBB(std::string a_Path) {
			return CreateFile(
				a_Path.c_str(), 
				FILE_APPEND_DATA, 
				FILE_SHARE_READ, 
				NULL, 
				OPEN_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL);
		}

		void WriteToFileBB(BBFILE a_File, std::string a_Message)
		{
			WriteFile(a_File, a_Message.c_str(), a_Message.length(), NULL, NULL);
		}

		void CloseFileBB(BBFILE a_File)
		{
			CloseHandle(a_File);
		}

	}
}