#include "System/FileHandler.h"
#include "Vector3.h"
#include "DataTypes/SharedRenderTypes.h"

struct Mesh {
	const char* name;
	Vertex* vertices;
	uint32_t vertAmount;
	unsigned short* indices;
	uint32_t indicesAmount;
};

struct Node
{
	Mesh mesh;
	Vector3 translation;
	//quads
	Vector3 Scale;
	//Matrix
};

struct GLTFFile {
	Node* nodes;
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