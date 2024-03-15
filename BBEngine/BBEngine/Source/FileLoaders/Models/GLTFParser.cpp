#include "FileLoaders/Models/GLTFParser.h"

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
		gltfFile->gltfPath = a_GLTFPath;

		char GLTFPath[64] = "";
		strcat(GLTFPath, a_GLTFPath);
		strcat(GLTFPath, a_GLTFName);
		JsonParser parser(GLTFPath);
		
		char binPath[64] = "";
		strcat(binPath, a_GLTFPath);
		strcat(binPath, parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);
		
		BBE::JSONList& AllNodes = parser.GetRootNode()["nodes"]->GetListBB();
		gltfFile->nodes = reinterpret_cast<Node*>(malloc(AllNodes.size() * sizeof(Node)));
		gltfFile->nodeAmount = AllNodes.size();
		gltfFile->PrimitiveCount = 0;

		//Note(Stan):	Currently only supporting one scene
		//				But this should be scalable
		BBE::JSONList& sceneList = parser.GetRootNode()["scenes"]->GetListBB();
		for (uint32_t curScene = 0; curScene < 1; curScene++)
		{
			BBE::JSONList& sceneNodesList = sceneList[curScene]->GetObjectBB()["nodes"]->GetListBB();
			for (uint32_t sceneHeadNodes = 0; sceneHeadNodes < sceneNodesList.size(); sceneHeadNodes++)
			{

			}

			//Go through all nodes in scene
			for (size_t i = 0; i < gltfFile->nodeAmount; i++)
			{
				uint32_t curMeshIndex = AllNodes[i]->GetObjectBB()["mesh"]->GetFloatBB();
				BBE::JSONObject curMesh = parser.GetRootNode()["meshes"]->GetListBB()[curMeshIndex]->GetObjectBB();

				gltfFile->nodes[i].translation = Vector3(0, 0, 0);
				if (AllNodes[i]->GetObjectBB()["translation"])
				{
					BBE::JSONList list = AllNodes[i]->GetObjectBB()["translation"]->GetListBB();
					gltfFile->nodes[i].translation = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
				}

				//Assign mesh name
				//Note(Stan):	This only works when doing it indirectly,
				//				Look into a way of fixing this.
				gltfFile->nodes[i].mesh.name = "";

				if (curMesh["name"])
				{
					std::string test = curMesh["name"]->GetStringBB();
					gltfFile->nodes[i].mesh.name = test.c_str();
				}

				JSONList& accessorsList = parser.GetRootNode()["accessors"]->GetListBB();
				JSONList& bufferViews = parser.GetRootNode()["bufferViews"]->GetListBB();
				JSONList& materials = parser.GetRootNode()["materials"]->GetListBB();
				JSONList& textures = parser.GetRootNode()["textures"]->GetListBB();
				JSONList& images = parser.GetRootNode()["images"]->GetListBB();

				//Go over all primitives
				BBE::JSONList& primitiveList = curMesh["primitives"]->GetListBB();
				gltfFile->nodes[i].mesh.primitiveCount = primitiveList.size();
				gltfFile->nodes[i].mesh.primative = reinterpret_cast<Mesh::Primative*>(malloc(gltfFile->nodes[i].mesh.primitiveCount * sizeof(Mesh::Primative)));
				for (uint32_t primitiveIndex = 0; primitiveIndex < gltfFile->nodes[i].mesh.primitiveCount; primitiveIndex++)
				{
					JSONObject& primitiveObj = primitiveList[primitiveIndex]->GetObjectBB();
					gltfFile->PrimitiveCount++;

					constexpr int NumOfAttibutes = 4;
					const char* attributes[NumOfAttibutes] = { {"POSITION"}, {"TEXCOORD_0"}, {"NORMAL"}, {"TANGENT"} };

					if (primitiveObj["attributes"])
					{
						//Note(Stan): Don't assume these are here..
						JSONObject& attributeObject = primitiveObj["attributes"]->GetObjectBB();

						for (size_t curAttibute = 0; curAttibute < NumOfAttibutes; curAttibute++)
						{
							gltfFile->nodes[i].mesh.primative[primitiveIndex].countData[curAttibute] = ParseAttribute(&gltfFile->nodes[i].mesh.primative[primitiveIndex].attributeData[curAttibute], attributeObject, accessorsList, bufferViews, attributes[curAttibute]);
						}

						gltfFile->totalVertexCount += gltfFile->nodes[i].mesh.primative[primitiveIndex].vertexCount;
					}

					if (primitiveObj["material"])
					{
						uint32_t materialIndex = primitiveObj["material"]->GetFloatBB();

						if (materials[materialIndex]->GetObjectBB()["pbrMetallicRoughness"])
						{
							JSONObject pbrMetallicRoughnessObj = materials[materialIndex]->GetObjectBB()["pbrMetallicRoughness"]->GetObjectBB();

							if (pbrMetallicRoughnessObj["baseColorFactor"])
							{
								JSONList baseColorFactor = pbrMetallicRoughnessObj["baseColorFactor"]->GetListBB();
								gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorFactor =
									Vector3(
										baseColorFactor[0]->GetFloatBB(),
										baseColorFactor[1]->GetFloatBB(),
										baseColorFactor[2]->GetFloatBB()
									);
							}

							if (pbrMetallicRoughnessObj["baseColorTexture"])
							{
								uint32_t baseColorIndex = pbrMetallicRoughnessObj["baseColorTexture"]->GetObjectBB()["index"]->GetFloatBB();
								std::string str = images[(uint32_t)textures[baseColorIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

								char* charPointer = (char*)malloc(str.size());
								strcpy(charPointer, str.c_str());
								gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorTexture.image.m_Path = charPointer;
							}

							if (pbrMetallicRoughnessObj["metallicRoughnessTexture"])
							{
								uint32_t metallicRoughnessTexture = pbrMetallicRoughnessObj["metallicRoughnessTexture"]->GetObjectBB()["index"]->GetFloatBB();
								std::string str = images[(uint32_t)textures[metallicRoughnessTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

								char* charPointer = (char*)malloc(str.size());
								strcpy(charPointer, str.c_str());
								gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.metallicRoughnessTexture.image.m_Path = charPointer;
							}

							if (pbrMetallicRoughnessObj["metallicFactor"])
							{
								gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.metallicFactor =
									static_cast<uint32_t>(pbrMetallicRoughnessObj["metallicFactor"]->GetFloatBB());
							}

							if (pbrMetallicRoughnessObj["roughnessFactor"])
							{
								gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.roughnessFactor =
									static_cast<uint32_t>(pbrMetallicRoughnessObj["roughnessFactor"]->GetFloatBB());
							}
						}

						if (materials[materialIndex]->GetObjectBB()["normalTexture"])
						{
							uint32_t normalTextureIndex = materials[materialIndex]->GetObjectBB()["normalTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = images[(uint32_t)textures[normalTextureIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.normalTexture.image.m_Path = charPointer;
						}

						if (materials[materialIndex]->GetObjectBB()["occlusionTexture"])
						{
							uint32_t occlusionTexture = materials[materialIndex]->GetObjectBB()["occlusionTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = images[(uint32_t)textures[occlusionTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.emissiveTexture.image.m_Path = charPointer;
						}

						if (materials[materialIndex]->GetObjectBB()["emissiveTexture"])
						{
							uint32_t emissiveTexture = materials[materialIndex]->GetObjectBB()["emissiveTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = images[(uint32_t)textures[emissiveTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.emissiveTexture.image.m_Path = charPointer;
						}

						if (materials[materialIndex]->GetObjectBB()["emissiveFactor"])
						{
							JSONList emissiveFactor = materials[materialIndex]->GetObjectBB()["emissiveFactor"]->GetListBB();
							gltfFile->nodes[i].mesh.primative[primitiveIndex].Material.emissiveFactor =
								Vector3(
									emissiveFactor[0]->GetFloatBB(),
									emissiveFactor[1]->GetFloatBB(),
									emissiveFactor[2]->GetFloatBB()
								);
						}
					}

					gltfFile->nodes[i].mesh.primative[primitiveIndex].indicesAmount = ParseAttribute(reinterpret_cast<void**>(&gltfFile->nodes[i].mesh.primative[primitiveIndex].indices), primitiveObj, accessorsList, bufferViews, "indices");
				}
			}
		}

		return gltfFile;
	}

	//TODO(Stan):	Currently, I am passing a lot of values as reference that could be
	//				defined somewhere more global
	uint32_t GLTFParser::ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, JSONList& a_AccessorList, JSONList& a_BufferViews, const char* a_Attribute)
	{
		uint32_t accessorIndex = 0;
		uint32_t accessorByteOffset = 0;


		uint32_t bufferViewIndex = 0;
		uint32_t bufferCount = 0;
		uint32_t byteLength = 0;
		uint32_t byteOffset = 0;

		if (a_AttributeObject[a_Attribute])
		{
			accessorIndex = static_cast<int>(a_AttributeObject[a_Attribute]->GetFloatBB());
			bufferViewIndex = static_cast<int>(a_AccessorList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
			accessorByteOffset = static_cast<int>(a_AccessorList[accessorIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());

			bufferCount = static_cast<int>(a_AccessorList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

			byteLength = static_cast<int>(a_BufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());

			if (a_BufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"])
			{
				byteOffset = static_cast<int>(a_BufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
			}

			*a_Data = malloc(byteLength);
			BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(*a_Data), byteLength, byteOffset + accessorByteOffset);
		}

		return bufferCount;
	}
}