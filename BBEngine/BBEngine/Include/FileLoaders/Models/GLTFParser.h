#include "System/FileHandler.h"

namespace BBE {

	class GLTFParser {
	public:
		GLTFParser();
		GLTFParser(std::string m_GLTFPath, std::string m_BinFile);
		~GLTFParser();

		int Parse(std::string m_GLTFPath, std::string m_BinFile);
	
	private:
		BBSystem::BBFILE m_BinFile;
	};

}