#include "System/FileHandler.h"
#include "Vector3.h"
#include "DataTypes/SharedRenderTypes.h"

namespace BBE {

	class GLTFParser {
	public:
		GLTFParser();
		GLTFParser(char* m_GLTFPath, char* a_GLTFName);
		~GLTFParser();

		GLTFFile* Parse(char* m_GLTFPath, char* a_GLTFName);
	
	private:
		BBSystem::BBFILE m_BinFile;
	};

}