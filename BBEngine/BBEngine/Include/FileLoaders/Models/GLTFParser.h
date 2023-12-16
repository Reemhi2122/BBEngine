#include "System/FileHandler.h"
#include "Vector3.h"

struct Mesh {
	const char* name;
	Vector3* vertices;
	uint32_t vertAmount;
	unsigned short* indices;
	uint32_t indicesAmount;
};

struct GLTFFile {
	Mesh* meshes;
};

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