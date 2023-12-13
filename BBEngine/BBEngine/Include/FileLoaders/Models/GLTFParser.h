#include "System/FileHandler.h"
#include "Vector3.h"

namespace BBE {

	struct GLTFFile
	{
		Vector3* Vertices;
	};

	class GLTFParser {
	public:
		GLTFParser();
		GLTFParser(std::string m_GLTFPath, std::string m_BinFile);
		~GLTFParser();

		GLTFFile* Parse(std::string m_GLTFPath, std::string m_BinFile);
	
	private:
		BBSystem::BBFILE m_BinFile;
	};

}