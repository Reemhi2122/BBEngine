#include "System/FileHandler.h"

namespace BBE {

	class GLTFParser {
	public:
		GLTFParser();
		~GLTFParser();

		int Parse(char* a_Path);

	private:
		BBSystem::BBFILE m_GLTFFile;
		BBSystem::BBFILE m_BinFile;
		

	};

}