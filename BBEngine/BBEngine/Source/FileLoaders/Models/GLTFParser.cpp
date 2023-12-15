#include "FileLoaders/Models/GLTFParser.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {}

	GLTFParser::GLTFParser(char* m_GLTFPath, char* a_GLTFName) {
		Parse(m_GLTFPath, a_GLTFName);
	}

	GLTFParser::~GLTFParser() {}


	//TODO(Stan):	Convert this function to using custom allocation
	//				and look into best way of giving the file
	GLTFFile* GLTFParser::Parse(char* a_GLTFPath, char* a_GLTFName) {
		
		GLTFFile* gltfFile = reinterpret_cast<GLTFFile*>(malloc(sizeof(GLTFFile)));

		char GLTFPath[64] = "";
		strcat(GLTFPath, a_GLTFPath);
		strcat(GLTFPath, a_GLTFName);
		JsonParser parser(GLTFPath);
		
		char binPath[64] = "";
		strcat(binPath, a_GLTFPath);
		strcat(binPath, parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);

		BBE::JSONList& MeshList = parser.GetRootNode()["meshes"]->GetListBB();
		gltfFile->meshes = reinterpret_cast<Mesh*>(malloc(MeshList.size() * sizeof(Mesh)));
		for (uint32_t meshIndex = 0; meshIndex < MeshList.size(); meshIndex++)
		{
			//Assign mesh name
			gltfFile->meshes[meshIndex].name = MeshList[meshIndex]->GetObjectBB()["name"]->GetStringBB().c_str();

			//Go over all primitives
			BBE::JSONList& primitiveList = MeshList[meshIndex]->GetObjectBB()["primitives"]->GetListBB();
			for (uint32_t primitiveIndex = 0; primitiveIndex < primitiveList.size(); primitiveIndex++)
			{
				JSONObject& primitiveObj = primitiveList[primitiveIndex]->GetObjectBB();
				JSONList& accessorsList = parser.GetRootNode()["accessors"]->GetListBB();
				JSONList& bufferViews = parser.GetRootNode()["bufferViews"]->GetListBB();
				uint32_t accessorIndex;
				uint32_t bufferViewIndex;
				uint32_t bufferCount;
				uint32_t byteLength;
				uint32_t byteOffset;

				if (primitiveObj.find("attributes") != primitiveObj.end())
				{
					//Note(Stan): Don't assume these are here..
					
					JSONObject& attributeObject = primitiveObj["attributes"]->GetObjectBB();

					accessorIndex		= static_cast<int>(attributeObject["POSITION"]->GetFloatBB());
					bufferViewIndex		= static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
					bufferCount			= static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

					byteLength			= static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());
					byteOffset			= static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
					
					gltfFile->meshes[meshIndex].vertices = reinterpret_cast<Vector3*>(malloc(bufferCount * sizeof(Vector3)));
					BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(gltfFile->meshes->vertices), byteLength, byteOffset);


				}

				//// Indices
				accessorIndex		= static_cast<int>(primitiveObj["indices"]->GetFloatBB());
				bufferViewIndex		= static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
				bufferCount			= static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

				byteLength			= static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());
				byteOffset			= static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());

				gltfFile->meshes[meshIndex].indices = reinterpret_cast<unsigned short*>(malloc(bufferCount * sizeof(unsigned short)));
				BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(gltfFile->meshes->indices), byteLength, byteOffset);

				//Process material
			}

		}
		return gltfFile;
	}

}