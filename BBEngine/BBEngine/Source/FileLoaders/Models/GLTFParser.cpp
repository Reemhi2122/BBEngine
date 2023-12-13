#include "FileLoaders/Models/GLTFParser.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {}

	GLTFParser::GLTFParser(std::string a_GLTFPath) {
		Parse(a_GLTFPath);
	}

	GLTFParser::~GLTFParser() {}

	//TODO(Stan):	Look into loading a folder instead of
	//				a path to a single file because it can contain
	//				both a GLTF file and a .bin file.

	GLTFFile* GLTFParser::Parse(std::string a_GLTFPath) {
		
		GLTFFile* file = reinterpret_cast<GLTFFile*>(malloc(sizeof(GLTFFile)));

		JsonParser parser(a_GLTFPath.c_str());
		std::string binPath = "Assets/Models/Cube/glTF/";
		binPath.append(parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);
		
		int accessorIndex = (int)parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["primitives"]->GetListBB()[0]->GetObjectBB()["attributes"]->GetObjectBB()["POSITION"]->GetFloatBB();
		int bufferViewIndex = (int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB();
		int bufferCount = (int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["count"]->GetFloatBB();

		uint32_t byteLength = (int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB();
		uint32_t byteOffset = (int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB();
		
		file->Vertices = reinterpret_cast<Vector3*>(malloc(bufferCount));
		BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(file->Vertices), byteLength, byteOffset);

		// Indices
		accessorIndex =		(int)parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["primitives"]->GetListBB()[0]->GetObjectBB()["indices"]->GetFloatBB();
		bufferViewIndex =	(int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB();
		bufferCount =		(int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["count"]->GetFloatBB();

		byteLength =		(int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB();
		byteOffset =		(int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB();

		file->Indices = reinterpret_cast<uint32_t*>(malloc(bufferCount));
		BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(file->Indices), byteLength, byteOffset);


		return file;
	}

}