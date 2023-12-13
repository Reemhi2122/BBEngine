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

	GLTFFile* GLTFParser::Parse(std::string a_GLTFPath, std::string a_BinFile) {
		
		GLTFFile* file = reinterpret_cast<GLTFFile*>(malloc(sizeof(GLTFFile)));

		JsonParser parser(a_GLTFPath.c_str());
		m_BinFile = BBSystem::OpenFileReadBB(a_BinFile.c_str());
		
		int accessorIndex = (int)parser.GetRootNode()["meshes"]->GetListBB()[0]->GetObjectBB()["primitives"]->GetListBB()[0]->GetObjectBB()["attributes"]->GetObjectBB()["POSITION"]->GetFloatBB();
		int bufferViewIndex = (int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB();
		int bufferCount = (int)parser.GetRootNode()["accessors"]->GetListBB()[accessorIndex]->GetObjectBB()["count"]->GetFloatBB();

		uint32_t byteLength = (int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB();
		uint32_t byteOffset = (int)parser.GetRootNode()["bufferViews"]->GetListBB()[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB();
		
		unsigned char* buffer = (unsigned char*)malloc(byteLength);
		BBSystem::ReadFileAtBB(m_BinFile, buffer, byteLength, byteOffset);

		file->Vertices = reinterpret_cast<Vector3*>(malloc(bufferCount));
		float* vertBuffer = reinterpret_cast<float*>(buffer);

		uint32_t vertIndex = 0;
		for (size_t i = 0; i < bufferCount; i++)
		{
			//file->Vertices[i] = Vector3(vertBuffer[vertIndex++], vertBuffer[vertIndex++], vertBuffer[vertIndex++]);
		}

		return file;
	}

}