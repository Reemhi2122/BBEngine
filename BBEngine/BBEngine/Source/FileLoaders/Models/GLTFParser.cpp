#include "FileLoaders/Models/GLTFParser.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {
		
	}

	GLTFParser::~GLTFParser() {
		
	}

	//TODO(Stan):	Look into loading a folder instead of
	//				a path to a single file because it can contain
	//				both a GLTF file and a .bin file.

	int GLTFParser::Parse(char* a_Path) {
		//bin file

		JsonParser parser(a_Path);
		std::string str = parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["name"]->GetStringBB();

		return 0;
	}

}