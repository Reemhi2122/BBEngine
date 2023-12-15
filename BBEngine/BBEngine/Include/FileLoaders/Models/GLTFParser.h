#include "System/FileHandler.h"
#include "Vector3.h"

namespace BBE {

	struct Mesh
	{
		const char* name;
		Vector3* vertices;
		unsigned short* indices;
	};

	struct GLTFFile
	{
		Mesh* meshes;
	};

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