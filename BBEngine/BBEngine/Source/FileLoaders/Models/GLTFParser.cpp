#include "FileLoaders/Models/GLTFParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {}
	GLTFParser::~GLTFParser() {}

	//TODO(Stan):	Convert this function to using custom allocation
	//				and look into best way of giving the file
	bool GLTFParser::Parse(char* a_GLTFPath, char* a_GLTFName, GLTFFile* a_GLTFFile) 
	{
		if (a_GLTFFile == nullptr)
			return false;

		m_GLTFFile = a_GLTFFile;
		m_GLTFFile->gltfPath = a_GLTFPath;

		char GLTFPath[64] = "";
		strcat(GLTFPath, a_GLTFPath);
		strcat(GLTFPath, a_GLTFName);
		
		m_Parser.Clear();
		m_Parser.Parse(GLTFPath);

		char binPath[64] = "";
		strcat(binPath, a_GLTFPath);
		strcat(binPath, m_Parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);

		m_AllNodes = m_Parser.GetRootNode()["nodes"]->GetListBB();
		m_GLTFFile->nodes = reinterpret_cast<Node*>(malloc(m_AllNodes.size() * sizeof(Node)));
		memset(m_GLTFFile->nodes, 0, m_AllNodes.size() * sizeof(Node));

		m_GLTFFile->nodeAmount = m_AllNodes.size();
		m_GLTFFile->PrimitiveCount = 0;

		if (m_GLTFFile->nodes == nullptr)
			return false;

		m_CurAccessorsList = m_Parser.GetRootNode()["accessors"]->GetListBB();
		m_CurBufferViews = m_Parser.GetRootNode()["bufferViews"]->GetListBB();
		m_CurMaterials = m_Parser.GetRootNode()["materials"]->GetListBB();
		m_CurTextures = m_Parser.GetRootNode()["textures"]->GetListBB();
		m_CurImages = m_Parser.GetRootNode()["images"]->GetListBB();

		BBE::JSONList& sceneList = m_Parser.GetRootNode()["scenes"]->GetListBB();
		for (uint32_t curScene = 0; curScene < sceneList.size(); curScene++)
		{
			BBE::JSONList& sceneNodesList = sceneList[curScene]->GetObjectBB()["nodes"]->GetListBB();
			for (uint32_t sceneHeadNodeIndex = 0; sceneHeadNodeIndex < sceneNodesList.size(); sceneHeadNodeIndex++)
			{
				uint32_t curIndex = sceneNodesList[sceneHeadNodeIndex]->GetFloatBB();
				BBE::Node* curNode = &m_GLTFFile->nodes[curIndex];

				CalculateNode(curNode, curIndex);
			}
		}

		return true;
	}

	void GLTFParser::CalculateNode(BBE::Node* a_CurNode, uint32_t a_CurNodeIndex)
	{
		a_CurNode->translation = Vector3(0.0, 0.0, 0.0);
		a_CurNode->rotation = Vector4(0.0, 0.0, 0.0, 1.0);
		a_CurNode->scale = Vector3(1.0, 1.0, 1.0);
		a_CurNode->mesh = {};

		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["translation"])
		{
			BBE::JSONList list = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["translation"]->GetListBB();
			a_CurNode->translation = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
		}

		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["rotation"])
		{
			BBE::JSONList list = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["rotation"]->GetListBB();
			a_CurNode->rotation = Vector4(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB(), list[3]->GetFloatBB());
		}

		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["scale"])
		{
			BBE::JSONList list = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["scale"]->GetListBB();
			a_CurNode->scale = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
		}

		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["mesh"]) {
			uint32_t curMeshIndex = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["mesh"]->GetFloatBB();
			BBE::JSONObject curMesh = m_Parser.GetRootNode()["meshes"]->GetListBB()[curMeshIndex]->GetObjectBB();
			
			//Note(Stan):	This only works when doing it indirectly,
			//				Look into a way of fixing this.
			a_CurNode->mesh.name = "Unnamed";
			if (curMesh["name"])
			{
				std::string meshName = curMesh["name"]->GetStringBB();
				a_CurNode->mesh.name = meshName.c_str();
			}

			//Go over all primitives
			BBE::JSONList& primitiveList = curMesh["primitives"]->GetListBB();
			a_CurNode->mesh.primitiveCount = primitiveList.size();
			a_CurNode->mesh.primative = reinterpret_cast<Mesh::Primative*>(malloc(a_CurNode->mesh.primitiveCount * sizeof(Mesh::Primative)));
			memset(a_CurNode->mesh.primative, 0, a_CurNode->mesh.primitiveCount * sizeof(Mesh::Primative));
			for (uint32_t primitiveIndex = 0; primitiveIndex < a_CurNode->mesh.primitiveCount; primitiveIndex++)
			{
				JSONObject& primitiveObj = primitiveList[primitiveIndex]->GetObjectBB();
				m_GLTFFile->PrimitiveCount++;

				constexpr int NumOfAttibutes = 4;
				const char* attributes[NumOfAttibutes] = { {"POSITION"}, {"TEXCOORD_0"}, {"NORMAL"}, {"TANGENT"} };

				if (primitiveObj["attributes"])
				{
					JSONObject& attributeObject = primitiveObj["attributes"]->GetObjectBB();

					for (size_t curAttibute = 0; curAttibute < NumOfAttibutes; curAttibute++)
					{
						a_CurNode->mesh.primative[primitiveIndex].countData[curAttibute] = ParseAttribute(&a_CurNode->mesh.primative[primitiveIndex].attributeData[curAttibute], attributeObject, attributes[curAttibute]);
					}

					m_GLTFFile->totalVertexCount += a_CurNode->mesh.primative[primitiveIndex].vertexCount;
				}

				if (primitiveObj["material"])
				{
					uint32_t materialIndex = primitiveObj["material"]->GetFloatBB();
					if (m_CurMaterials[materialIndex]->GetObjectBB()["pbrMetallicRoughness"])
					{
						JSONObject pbrMetallicRoughnessObj = m_CurMaterials[materialIndex]->GetObjectBB()["pbrMetallicRoughness"]->GetObjectBB();

						if (pbrMetallicRoughnessObj["baseColorFactor"])
						{
							JSONList baseColorFactor = pbrMetallicRoughnessObj["baseColorFactor"]->GetListBB();
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorFactor =
								Vector4(
									baseColorFactor[0]->GetFloatBB(),
									baseColorFactor[1]->GetFloatBB(),
									baseColorFactor[2]->GetFloatBB(),
									baseColorFactor[3]->GetFloatBB()
								);
						}

						if (pbrMetallicRoughnessObj["baseColorTexture"])
						{
							uint32_t baseColorIndex = pbrMetallicRoughnessObj["baseColorTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[baseColorIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorTexture.path = charPointer;
						}

						if (pbrMetallicRoughnessObj["metallicRoughnessTexture"])
						{
							uint32_t metallicRoughnessTexture = pbrMetallicRoughnessObj["metallicRoughnessTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[metallicRoughnessTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.metallicRoughnessTexture.path = charPointer;
						}

						if (pbrMetallicRoughnessObj["metallicFactor"])
						{
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.metallicFactor =
								static_cast<uint32_t>(pbrMetallicRoughnessObj["metallicFactor"]->GetFloatBB());
						}

						if (pbrMetallicRoughnessObj["roughnessFactor"])
						{
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.roughnessFactor =
								static_cast<uint32_t>(pbrMetallicRoughnessObj["roughnessFactor"]->GetFloatBB());
						}
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["normalTexture"])
					{
						JSONObject normalTextureObject = m_CurMaterials[materialIndex]->GetObjectBB()["normalTexture"]->GetObjectBB();

						if (normalTextureObject["index"])
						{
							uint32_t normalTextureIndex = normalTextureObject["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[normalTextureIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.normalTexture.path = charPointer;
						}

						if (normalTextureObject["scale"])
						{
							a_CurNode->mesh.primative->Material.normalTextureScale = normalTextureObject["scale"]->GetFloatBB();
						}

						//Todo(Stan): Still need to add possiblility for texture coords here.
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["occlusionTexture"])
					{
						JSONObject occlusionTextureObject = m_CurMaterials[materialIndex]->GetObjectBB()["occlusionTexture"]->GetObjectBB();

						if (occlusionTextureObject["index"])
						{
							uint32_t occlusionTexture = occlusionTextureObject["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[occlusionTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.occlusionTexture.path = charPointer;
						}
						
						if (occlusionTextureObject["strength"])
						{
							a_CurNode->mesh.primative->Material.occlusionTextureStrength = occlusionTextureObject["scale"]->GetFloatBB();
						}

						//Todo(Stan): Still need to add possiblility for texture coords here.
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["emissiveTexture"])
					{
						JSONObject emissiveTextureObject = m_CurMaterials[materialIndex]->GetObjectBB()["emissiveTexture"]->GetObjectBB();

						if (emissiveTextureObject["index"])
						{
							uint32_t emissiveTexture = emissiveTextureObject["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[emissiveTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();
							
							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.emissiveTexture.path = charPointer;

						}

						//Todo(Stan): Still need to add possiblility for texture coords here.
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["emissiveFactor"])
					{
						JSONList emissiveFactor = m_CurMaterials[materialIndex]->GetObjectBB()["emissiveFactor"]->GetListBB();
						a_CurNode->mesh.primative[primitiveIndex].Material.emissiveFactor =
							Vector3(
								emissiveFactor[0]->GetFloatBB(),
								emissiveFactor[1]->GetFloatBB(),
								emissiveFactor[2]->GetFloatBB()
							);
					}

					a_CurNode->mesh.primative[primitiveIndex].Material.alphaMode = AlphaMode::OPAQUE_MODE;
					if (m_CurMaterials[materialIndex]->GetObjectBB()["alphaMode"])
					{
						std::string alphaBlend = m_CurMaterials[materialIndex]->GetObjectBB()["alphaMode"]->GetStringBB();
						
						if (strcmp(alphaBlend.c_str(), "BLEND") == 0)
						{
							a_CurNode->mesh.primative[primitiveIndex].Material.alphaMode = AlphaMode::BLEND_MODE;
						}
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["extensions"])
					{
						JSONObject extensionsObject = m_CurMaterials[materialIndex]->GetObjectBB()["extensions"]->GetObjectBB();

						// EXTENSION: KHR_materials_transmission
						if (extensionsObject["KHR_materials_transmission"])
						{
							JSONObject khrTransmissionObject = extensionsObject["KHR_materials_transmission"]->GetObjectBB();
							a_CurNode->mesh.primative[primitiveIndex].Material.extensions.hasKhrMaterialTransmission = true;

							if (khrTransmissionObject["transmissionFactor"])
							{
								a_CurNode->mesh.primative[primitiveIndex].Material.extensions.khrMaterialTransmission.transmissionFactor = khrTransmissionObject["transmissionFactor"]->GetFloatBB();
							}

							if (khrTransmissionObject["transmissionTexture"])
							{
								uint32_t transmissionTextureIndex = khrTransmissionObject["transmissionTexture"]->GetObjectBB()["index"]->GetFloatBB();
								std::string uri = m_CurImages[(uint32_t)m_CurTextures[transmissionTextureIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

								char* charPointer = (char*)malloc(uri.size());
								strcpy(charPointer, uri.c_str());
								a_CurNode->mesh.primative[primitiveIndex].Material.extensions.khrMaterialTransmission.transmissionTexture.path = charPointer;
							}
						}

						// EXTENSION: KHR_materials_volume
						if (extensionsObject["KHR_materials_volume"])
						{
							JSONObject khrvolumeObject = extensionsObject["KHR_materials_volume"]->GetObjectBB();
							a_CurNode->mesh.primative[primitiveIndex].Material.extensions.hasKhrMaterialVolume = true;

							Mesh::Primative::Material::Extensions::KhrMaterialvolume& khrMaterialVolume = a_CurNode->mesh.primative[primitiveIndex].Material.extensions.khrMaterialVolume;

							khrMaterialVolume.thicknessFactor = 0;
							if (khrvolumeObject["thicknessFactor"])
							{
								khrMaterialVolume.thicknessFactor = khrvolumeObject["thicknessFactor"]->GetFloatBB();
							}

							if (khrvolumeObject["thicknessTexture"])
							{
								uint32_t thicknessTextureIndex = khrvolumeObject["thicknessTexture"]->GetObjectBB()["index"]->GetFloatBB();
								std::string uri = m_CurImages[(uint32_t)m_CurTextures[thicknessTextureIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

								char* charPointer = (char*)malloc(uri.size());
								strcpy(charPointer, uri.c_str());
								khrMaterialVolume.thicknessTexture.path = charPointer;
							}

							khrMaterialVolume.thicknessFactor = +INFINITY;
							if (khrvolumeObject["attenuationDistance"])
							{
								khrMaterialVolume.attenuationDistance = khrvolumeObject["attenuationDistance"]->GetFloatBB();
							}

							khrMaterialVolume.attenuationColor = Vector3(1.0f, 1.0f, 1.0f);
							if (khrvolumeObject["attenuationColor"])
							{
								JSONList thicknessColorFactor = khrvolumeObject["attenuationColor"]->GetListBB();
								khrMaterialVolume.attenuationColor =
									Vector3(
										thicknessColorFactor[0]->GetFloatBB(),
										thicknessColorFactor[1]->GetFloatBB(),
										thicknessColorFactor[2]->GetFloatBB()
									);
							}
						}
					}
				}

				a_CurNode->mesh.primative[primitiveIndex].indicesAmount = 
					ParseAttribute(
						reinterpret_cast<void**>(&a_CurNode->mesh.primative[primitiveIndex].indices), 
						primitiveObj, 
						"indices", 
						&a_CurNode->mesh.primative[primitiveIndex].indicesDataSize);
			}
		}

		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["children"])
		{
			BBE::JSONList childNodes = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["children"]->GetListBB();
			for (size_t childNodeIndex = 0; childNodeIndex < childNodes.size(); childNodeIndex++)
			{
				a_CurNodeIndex = childNodes[childNodeIndex]->GetFloatBB();
				BBE::Node* node = &m_GLTFFile->nodes[a_CurNodeIndex];
				node->Parent = a_CurNode;
				CalculateNode(node, a_CurNodeIndex);
			}
		}
	}

	static uint8_t GetElementSize(uint32_t a_ComponentType)
	{
		switch (a_ComponentType)
		{
		case 5120:
		case 5121:
			return 1;
			break;
		case 5122:
		case 5123:
			return 2;
			break;
		case 5125:
		case 5126:
			return 4;
			break;
		default:
			break;
		}
	}

	uint32_t GLTFParser::ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, const char* a_Attribute, uint8_t* a_DataSize)
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
			bufferViewIndex = static_cast<int>(m_CurAccessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());

			if (a_DataSize)
			{
				*a_DataSize = GetElementSize(m_CurAccessorsList[accessorIndex]->GetObjectBB()["componentType"]->GetFloatBB());
			}

			if (m_CurAccessorsList[accessorIndex]->GetObjectBB()["byteOffset"]) {
				accessorByteOffset = static_cast<int>(m_CurAccessorsList[accessorIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
			}

			bufferCount = static_cast<int>(m_CurAccessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

			byteLength = static_cast<int>(m_CurBufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());

			if (m_CurBufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"])
			{
				byteOffset = static_cast<int>(m_CurBufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
			}

			*a_Data = malloc(byteLength);
			BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(*a_Data), byteLength, byteOffset + accessorByteOffset);
		}

		return bufferCount;
	}
}