#include "FileLoaders/Models/GLTFParser.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {}

	GLTFParser::GLTFParser(char* m_GLTFPath, char* a_GLTFName) {
		Parse(m_GLTFPath, a_GLTFName);
	}

	GLTFParser::~GLTFParser() {}

	//TODO(Stan):	Look into loading a folder instead of
	//				a path to a single file because it can contain
	//				both a GLTF file and a .bin file.

	GLTFFile* GLTFParser::Parse(char* a_GLTFPath, char* a_GLTFName) {
		
		GLTFFile* file = reinterpret_cast<GLTFFile*>(malloc(sizeof(GLTFFile)));

		char GLTFPath[64] = "";
		strcat(GLTFPath, a_GLTFPath);
		strcat(GLTFPath, a_GLTFName);
		JsonParser parser(GLTFPath);
		
		char binPath[64] = "";
		strcat(binPath, a_GLTFPath);
		strcat(binPath, parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);
		
		int accessorIndex = (int)parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["primitives"]->GetListBB()[0]->GetObjectBB()["attributes"]->GetObjectBB()["POSITION"]->GetFloatBB();
		int bufferViewIndex = (int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB();
		int bufferCount = (int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["count"]->GetFloatBB();

		uint32_t byteLength = (int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB();
		uint32_t byteOffset = (int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB();
		
		file->Vertices = reinterpret_cast<Vector3*>(malloc(bufferCount * sizeof(Vector3)));
		BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(file->Vertices), byteLength, byteOffset);

		// Indices
		accessorIndex =		(int)parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["primitives"]->GetListBB()[0]->GetObjectBB()["indices"]->GetFloatBB();
		bufferViewIndex =	(int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB();
		bufferCount =		(int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["count"]->GetFloatBB();

		byteLength =		(int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB();
		byteOffset =		(int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB();

		file->Indices = reinterpret_cast<unsigned short*>(malloc(bufferCount * sizeof(unsigned short)));
		BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(file->Indices), byteLength, byteOffset);

		return file;
	}

}