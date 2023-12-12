#include "FileLoaders/Models/GLTFParser.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {}

	GLTFParser::GLTFParser(std::string a_GLTFPath, std::string a_BinFile) {
		Parse(a_GLTFPath, a_BinFile);
	}

	GLTFParser::~GLTFParser() {}

	//TODO(Stan):	Look into loading a folder instead of
	//				a path to a single file because it can contain
	//				both a GLTF file and a .bin file.

	int GLTFParser::Parse(std::string a_GLTFPath, std::string a_BinFile) {
		
		JsonParser parser(a_GLTFPath.c_str());
		m_BinFile = BBSystem::OpenFileWriteBB(a_BinFile.c_str());
		
		std::string str = parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["name"]->GetStringBB();



		return 0;
	}

}