#pragma once
#include <string>

namespace BBE {
	namespace BBSystem {
		
		using DWORD = unsigned long;
		using BBFILE = void*;

		void CreateDirBB(std::string path);
		bool FileExistsBB(std::string path);

		BBFILE CreateFileBB(std::string a_Path);
		BBFILE OpenFileBB(std::string a_Path);
		void WriteToFileBB(BBFILE a_File, std::string a_Message);
		void CloseFileBB(BBFILE a_File);
	}
}