#include "System/FileHandler.h"
#include "Vector3.h"

namespace BBE {

	struct GLTFFile
	{
		Vector3* Vertices;
		uint32_t* Indices;
	};

	class GLTFParser {
	public:
		GLTFParser();
		GLTFParser(std::string m_GLTFPath);
		~GLTFParser();

		GLTFFile* Parse(std::string m_GLTFPath);
	
	private:
		BBSystem::BBFILE m_BinFile;
	};

}