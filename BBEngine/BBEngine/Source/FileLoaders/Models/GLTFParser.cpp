#include "FileLoaders/Models/GLTFParser.h"

namespace BBE {

	GLTFParser::GLTFParser() {}
	GLTFParser::~GLTFParser() {}

	//TODO(Stan):	Convert this function to using custom allocation
	//				and look into best way of giving the file
	bool GLTFParser::Parse(const char* a_GLTFPath, const char* a_GLTFName, GLTFFile* a_GLTFFile) 
	{
		if (a_GLTFFile == nullptr)
		{
			printf("[GLTF Parser]: GLTF file is a nullptr!");
			return false;
		}

		if (a_GLTFPath == nullptr)
		{
			printf("[GLTF Parser]: GLTF path is a nullptr!");
			return false;
		}

		m_GLTFFile = a_GLTFFile;
		m_GLTFFile->gltfPath = a_GLTFPath;

		char GLTFPath[FILE_PATH_SIZE] = "";
		strcat(GLTFPath, a_GLTFPath);
		strcat(GLTFPath, a_GLTFName);
		
		m_Parser.Clear();
		m_Parser.Parse(GLTFPath);

		char binPath[FILE_PATH_SIZE] = "";
		strcat(binPath, a_GLTFPath);
		strcat(binPath, m_Parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);

		m_AllNodes = m_Parser.GetRootNode()["nodes"]->GetListBB();
		m_GLTFFile->nodes = reinterpret_cast<GLTFNode*>(malloc(m_AllNodes.size() * sizeof(GLTFNode)));
		if (!m_GLTFFile->nodes)
		{
			printf("[GLTF Parser]: Failed to allocate memory for %lli amount of nodes", m_AllNodes.size());
			return false;
		}

		memset(m_GLTFFile->nodes, 0, m_AllNodes.size() * sizeof(GLTFNode));
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
			m_GLTFFile->rootNodeAmount = sceneNodesList.size();
			for (uint32_t sceneHeadNodeIndex = 0; sceneHeadNodeIndex < sceneNodesList.size(); sceneHeadNodeIndex++)
			{
				uint32_t curIndex = sceneNodesList[sceneHeadNodeIndex]->GetFloatBB();
				BBE::GLTFNode* curNode = &m_GLTFFile->nodes[curIndex];
				curNode->NodeIndex = curIndex;
				a_GLTFFile->rootNode = curNode;
				CalculateNode(curNode, curIndex);
			}
		}

		return true;
	}

	void GLTFParser::CalculateNode(BBE::GLTFNode* a_CurNode, uint32_t a_CurNodeIndex)
	{
		a_CurNode->Parent = nullptr;
		a_CurNode->translation = Vector3(0.0, 0.0, 0.0);
		a_CurNode->rotation = Vector4(0.0, 0.0, 0.0, 1.0);
		a_CurNode->scale = Vector3(1.0, 1.0, 1.0);
		a_CurNode->mesh = {};

		strcpy(a_CurNode->name, "BBObject");
		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["name"])
		{
			std::string meshName = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["name"]->GetStringBB();
			strcpy(a_CurNode->name, meshName.c_str());
		}

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
			strcpy(a_CurNode->mesh.name, "Unnamed");
			if (curMesh["name"])
			{
				std::string meshName = curMesh["name"]->GetStringBB();
				strcpy(a_CurNode->mesh.name, meshName.c_str());
			}

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
					BBE::Mesh::Primative::Material& curPrimitiveMaterial = a_CurNode->mesh.primative[primitiveIndex].material;

					if (m_CurMaterials[materialIndex]->GetObjectBB()["pbrMetallicRoughness"])
					{
						JSONObject pbrMetallicRoughnessObj = m_CurMaterials[materialIndex]->GetObjectBB()["pbrMetallicRoughness"]->GetObjectBB();

						if (pbrMetallicRoughnessObj["baseColorFactor"])
						{
							JSONList baseColorFactor = pbrMetallicRoughnessObj["baseColorFactor"]->GetListBB();
							curPrimitiveMaterial.pbrMetallicRoughness.baseColorFactor =
								Vector4(
									baseColorFactor[0]->GetFloatBB(),
									baseColorFactor[1]->GetFloatBB(),
									baseColorFactor[2]->GetFloatBB(),
									baseColorFactor[3]->GetFloatBB()
								);
						}

						ParseTexture(
							pbrMetallicRoughnessObj,
							"baseColorTexture", nullptr,
							&curPrimitiveMaterial.pbrMetallicRoughness.baseColorTexture, nullptr);

						ParseTexture(
							pbrMetallicRoughnessObj,
							"metallicRoughnessTexture", nullptr,
							&curPrimitiveMaterial.pbrMetallicRoughness.metallicRoughnessTexture, nullptr);

						if (pbrMetallicRoughnessObj["metallicFactor"])
						{
							curPrimitiveMaterial.pbrMetallicRoughness.metallicFactor =
								static_cast<uint32_t>(pbrMetallicRoughnessObj["metallicFactor"]->GetFloatBB());
						}

						if (pbrMetallicRoughnessObj["roughnessFactor"])
						{
							curPrimitiveMaterial.pbrMetallicRoughness.roughnessFactor =
								static_cast<uint32_t>(pbrMetallicRoughnessObj["roughnessFactor"]->GetFloatBB());
						}
					}

					ParseTexture(
						m_CurMaterials[materialIndex]->GetObjectBB(), 
						"normalTexture", "scale", 
						&curPrimitiveMaterial.normalTexture, &curPrimitiveMaterial.normalTextureScale);
					
					ParseTexture(m_CurMaterials[materialIndex]->GetObjectBB(), 
						"occlusionTexture", "strength", 
						&curPrimitiveMaterial.occlusionTexture, &curPrimitiveMaterial.occlusionTextureStrength);

					ParseTexture(m_CurMaterials[materialIndex]->GetObjectBB(),
						"emissiveTexture", nullptr,
						&curPrimitiveMaterial.emissiveTexture, nullptr);
					
					if (m_CurMaterials[materialIndex]->GetObjectBB()["emissiveFactor"])
					{
						JSONList emissiveFactor = m_CurMaterials[materialIndex]->GetObjectBB()["emissiveFactor"]->GetListBB();
						curPrimitiveMaterial.emissiveFactor =
							Vector3(
								emissiveFactor[0]->GetFloatBB(),
								emissiveFactor[1]->GetFloatBB(),
								emissiveFactor[2]->GetFloatBB()
							);
					}

					curPrimitiveMaterial.alphaMode = AlphaMode::OPAQUE_MODE;
					if (m_CurMaterials[materialIndex]->GetObjectBB()["alphaMode"])
					{
						std::string alphaBlend = m_CurMaterials[materialIndex]->GetObjectBB()["alphaMode"]->GetStringBB();
						
						if (strcmp(alphaBlend.c_str(), "BLEND") == 0)
						{
							curPrimitiveMaterial.alphaMode = AlphaMode::BLEND_MODE;
						}
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["extensions"])
					{
						JSONObject extensionsObject = m_CurMaterials[materialIndex]->GetObjectBB()["extensions"]->GetObjectBB();

						// EXTENSION: KHR_materials_transmission
						if (extensionsObject["KHR_materials_transmission"])
						{
							JSONObject khrTransmissionObject = extensionsObject["KHR_materials_transmission"]->GetObjectBB();
							curPrimitiveMaterial.extensions.hasKhrMaterialTransmission = true;

							Mesh::Primative::Material::Extensions::KhrMaterialTransmission& khrMaterialTransmission = curPrimitiveMaterial.extensions.khrMaterialTransmission;
							curPrimitiveMaterial.extensions.khrMaterialTransmission.transmissionFactor = 0;
							ParseTexture(
								khrTransmissionObject,
								"transmissionTexture", "transmissionFactor",
								&khrMaterialTransmission.transmissionTexture, &khrMaterialTransmission.transmissionFactor);
						}

						// EXTENSION: KHR_materials_volume
						if (extensionsObject["KHR_materials_volume"])
						{
							JSONObject khrvolumeObject = extensionsObject["KHR_materials_volume"]->GetObjectBB();
							curPrimitiveMaterial.extensions.hasKhrMaterialVolume = true;

							Mesh::Primative::Material::Extensions::KhrMaterialvolume& khrMaterialVolume = curPrimitiveMaterial.extensions.khrMaterialVolume;
							khrMaterialVolume.thicknessFactor = 0;
							ParseTexture(
								khrvolumeObject,
								"thicknessTexture", "thicknessFactor",
								&khrMaterialVolume.thicknessTexture, &khrMaterialVolume.thicknessFactor);

							khrMaterialVolume.attenuationDistance = +INFINITY;
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
				BBE::GLTFNode* childNode = &m_GLTFFile->nodes[a_CurNodeIndex];
				childNode->NodeIndex = a_CurNodeIndex;
				childNode->Parent = a_CurNode;
				a_CurNode->Children.push_back(childNode);
				CalculateNode(childNode, a_CurNodeIndex);
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
			accessorIndex = static_cast<uint32_t>(a_AttributeObject[a_Attribute]->GetFloatBB());
			bufferViewIndex = static_cast<uint32_t>(m_CurAccessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());

			if (a_DataSize)
			{
				*a_DataSize = GetElementSize(m_CurAccessorsList[accessorIndex]->GetObjectBB()["componentType"]->GetFloatBB());
			}

			if (m_CurAccessorsList[accessorIndex]->GetObjectBB()["byteOffset"]) {
				accessorByteOffset = static_cast<uint32_t>(m_CurAccessorsList[accessorIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
			}

			bufferCount = static_cast<uint32_t>(m_CurAccessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

			byteLength = static_cast<uint32_t>(m_CurBufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());

			if (m_CurBufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"])
			{
				byteOffset = static_cast<uint32_t>(m_CurBufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
			}

			*a_Data = malloc(byteLength);
			BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(*a_Data), byteLength, byteOffset + accessorByteOffset);
		}

		return bufferCount;
	}

	uint32_t GLTFParser::ParseTexture(JSONObject a_CurObject, const char* a_TextureName, const char* a_AdditionalValueName, TextureT* a_PathOut, float* a_OutAditionalValue)
	{
		if (a_CurObject[a_TextureName])
		{
			JSONObject TextureObject = a_CurObject[a_TextureName]->GetObjectBB();

			// Check index (Should always be there)
			{
				//Todo(Stan): Might want to look into just saving index and saving full textures seperate
				uint32_t normalTextureIndex = TextureObject["index"]->GetFloatBB();
				std::string str = m_CurImages[(uint32_t)m_CurTextures[normalTextureIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

				char* charPointer = reinterpret_cast<char*>(malloc(str.size()));
				if (charPointer)
				{
					strcpy(charPointer, str.c_str());
					a_PathOut->path = charPointer;
				}
			}

			if (TextureObject["texCoord"])
			{
				a_PathOut->texCoordIndex= TextureObject["texCoord"]->GetFloatBB();
			}

			if (a_AdditionalValueName && TextureObject[a_AdditionalValueName])
			{
				(*a_OutAditionalValue) = TextureObject[a_AdditionalValueName]->GetFloatBB();
			}
		}

		return 1;
	}
}