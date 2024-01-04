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
		
		//Currently only supports one scene

		//Go through all nodes
		BBE::JSONList& nodesList = parser.GetRootNode()["nodes"]->GetListBB();
		gltfFile->nodes = reinterpret_cast<Node*>(malloc(nodesList.size() * sizeof(Node)));
		//TODO(Stan):	Added -1 to this because there is a non node object
		//				in the list. Look at this later.
		for (size_t i = 0; i < nodesList.size() - 1; i++)
		{
			uint32_t curMeshIndex = nodesList[i]->GetObjectBB()["mesh"]->GetFloatBB();
			BBE::JSONObject curMesh = parser.GetRootNode()["meshes"]->GetListBB()[curMeshIndex]->GetObjectBB();

			BBE::JSONList list = nodesList[i]->GetObjectBB()["translation"]->GetListBB();
			gltfFile->nodes[i].translation = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());

			//Assign mesh name
			//Note(Stan):	This only works when doing it indirectly,
			//				Look into a way of fixing this.
			std::string test = curMesh["name"]->GetStringBB();
			gltfFile->nodes[i].mesh.name = test.c_str();

			//Go over all primitives
			BBE::JSONList& primitiveList = curMesh["primitives"]->GetListBB();
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

					accessorIndex = static_cast<int>(attributeObject["POSITION"]->GetFloatBB());
					bufferViewIndex = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
					bufferCount = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

					byteLength = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());
					byteOffset = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());

					gltfFile->nodes[i].mesh.vertAmount = bufferCount;
					gltfFile->nodes[i].mesh.vertices = reinterpret_cast<Vector3*>(malloc(byteLength));
					BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(gltfFile->nodes[i].mesh.vertices), byteLength, byteOffset);
				}

				//Indices
				accessorIndex = static_cast<int>(primitiveObj["indices"]->GetFloatBB());
				bufferViewIndex = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
				bufferCount = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

				byteLength = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());
				byteOffset = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());

				gltfFile->nodes[i].mesh.indicesAmount = bufferCount;
				gltfFile->nodes[i].mesh.indices = reinterpret_cast<unsigned short*>(malloc(byteLength));
				BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(gltfFile->nodes[i].mesh.indices), byteLength, byteOffset);

				//Process material
			}

		}
		return gltfFile;
	}

}