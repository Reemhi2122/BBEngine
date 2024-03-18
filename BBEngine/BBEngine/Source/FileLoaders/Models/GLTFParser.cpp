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
		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["mesh"]) {
			uint32_t curMeshIndex = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["mesh"]->GetFloatBB();
			BBE::JSONObject curMesh = m_Parser.GetRootNode()["meshes"]->GetListBB()[curMeshIndex]->GetObjectBB();

			a_CurNode->translation = Vector3(0.0f, 0.0f, 0.0f);
			if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["translation"])
			{
				BBE::JSONList list = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["translation"]->GetListBB();
				a_CurNode->translation = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
			}

			a_CurNode->scale = Vector3(1.0f, 1.0f, 1.0f);
			if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["scale"])
			{
				BBE::JSONList list = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["scale"]->GetListBB();
				a_CurNode->scale = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
			}

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
								Vector3(
									baseColorFactor[0]->GetFloatBB(),
									baseColorFactor[1]->GetFloatBB(),
									baseColorFactor[2]->GetFloatBB()
								);
						}

						a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorTexture.enabled = false;
						if (pbrMetallicRoughnessObj["baseColorTexture"])
						{
							uint32_t baseColorIndex = pbrMetallicRoughnessObj["baseColorTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[baseColorIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorTexture.image.m_Path = charPointer;
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorTexture.enabled = true;
						}

						if (pbrMetallicRoughnessObj["metallicRoughnessTexture"])
						{
							uint32_t metallicRoughnessTexture = pbrMetallicRoughnessObj["metallicRoughnessTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[metallicRoughnessTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.metallicRoughnessTexture.image.m_Path = charPointer;
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
						uint32_t normalTextureIndex = m_CurMaterials[materialIndex]->GetObjectBB()["normalTexture"]->GetObjectBB()["index"]->GetFloatBB();
						std::string str = m_CurImages[(uint32_t)m_CurTextures[normalTextureIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

						char* charPointer = (char*)malloc(str.size());
						strcpy(charPointer, str.c_str());
						a_CurNode->mesh.primative[primitiveIndex].Material.normalTexture.image.m_Path = charPointer;
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["occlusionTexture"])
					{
						uint32_t occlusionTexture = m_CurMaterials[materialIndex]->GetObjectBB()["occlusionTexture"]->GetObjectBB()["index"]->GetFloatBB();
						std::string str = m_CurImages[(uint32_t)m_CurTextures[occlusionTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

						char* charPointer = (char*)malloc(str.size());
						strcpy(charPointer, str.c_str());
						a_CurNode->mesh.primative[primitiveIndex].Material.emissiveTexture.image.m_Path = charPointer;
					}

					if (m_CurMaterials[materialIndex]->GetObjectBB()["emissiveTexture"])
					{
						uint32_t emissiveTexture = m_CurMaterials[materialIndex]->GetObjectBB()["emissiveTexture"]->GetObjectBB()["index"]->GetFloatBB();
						std::string str = m_CurImages[(uint32_t)m_CurTextures[emissiveTexture]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

						char* charPointer = (char*)malloc(str.size());
						strcpy(charPointer, str.c_str());
						a_CurNode->mesh.primative[primitiveIndex].Material.emissiveTexture.image.m_Path = charPointer;
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
				}

				a_CurNode->mesh.primative[primitiveIndex].indicesAmount = ParseAttribute(reinterpret_cast<void**>(&a_CurNode->mesh.primative[primitiveIndex].indices), primitiveObj, "indices");
			}
		}

		if (m_AllNodes[a_CurNodeIndex]->GetObjectBB()["children"])
		{
			BBE::JSONList childNodes = m_AllNodes[a_CurNodeIndex]->GetObjectBB()["children"]->GetListBB();
			for (size_t childNodeIndex = 0; childNodeIndex < childNodes.size(); childNodeIndex++)
			{
				a_CurNodeIndex = childNodes[childNodeIndex]->GetFloatBB();
				BBE::Node* node = &m_GLTFFile->nodes[a_CurNodeIndex];
				CalculateNode(node, a_CurNodeIndex);
			}
		}
	}

	uint32_t GLTFParser::ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, const char* a_Attribute)
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