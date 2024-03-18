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
		if (gltfFile == nullptr)
			return nullptr;

		gltfFile->gltfPath = a_GLTFPath;

		char GLTFPath[64] = "";
		strcat(GLTFPath, a_GLTFPath);
		strcat(GLTFPath, a_GLTFName);
		m_Parser.Parse(GLTFPath);

		char binPath[64] = "";
		strcat(binPath, a_GLTFPath);
		strcat(binPath, m_Parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
		m_BinFile = BBSystem::OpenFileReadBB(binPath);

		BBE::JSONList& AllNodes = m_Parser.GetRootNode()["nodes"]->GetListBB();
		gltfFile->nodes = reinterpret_cast<Node*>(malloc(AllNodes.size() * sizeof(Node)));
		gltfFile->nodeAmount = AllNodes.size();
		gltfFile->PrimitiveCount = 0;

		if (gltfFile->nodes == nullptr)
			return nullptr;

		m_CurAccessorsList = m_Parser.GetRootNode()["accessors"]->GetListBB();
		m_CurBufferViews = m_Parser.GetRootNode()["bufferViews"]->GetListBB();
		m_CurMaterials = m_Parser.GetRootNode()["materials"]->GetListBB();
		m_CurTextures = m_Parser.GetRootNode()["textures"]->GetListBB();
		m_CurImages = m_Parser.GetRootNode()["images"]->GetListBB();

		//Note(Stan):	Currently only supporting one scene
		//				But this should be scalable
		BBE::JSONList& sceneList = m_Parser.GetRootNode()["scenes"]->GetListBB();
		for (uint32_t curScene = 0; curScene < 1; curScene++)
		{
			BBE::JSONList& sceneNodesList = sceneList[curScene]->GetObjectBB()["nodes"]->GetListBB();
			for (uint32_t sceneHeadNodeIndex = 0; sceneHeadNodeIndex < sceneNodesList.size(); sceneHeadNodeIndex++)
			{
				uint32_t curIndex = sceneNodesList[sceneHeadNodeIndex]->GetFloatBB();
				BBE::Node* curNode = &gltfFile->nodes[curIndex];

				CalculateNode(curNode, AllNodes, curIndex, gltfFile);
			}
		}

		return gltfFile;
	}

	void GLTFParser::CalculateNode(BBE::Node* a_CurNode, BBE::JSONList& a_AllNodes, uint32_t a_CurNodeIndex, BBE::GLTFFile* a_GLTFFile)
	{
		if (a_AllNodes[a_CurNodeIndex]->GetObjectBB()["mesh"]) {
			a_CurNode->ShouldRender = true;

			uint32_t curMeshIndex = a_AllNodes[a_CurNodeIndex]->GetObjectBB()["mesh"]->GetFloatBB();
			BBE::JSONObject curMesh = m_Parser.GetRootNode()["meshes"]->GetListBB()[curMeshIndex]->GetObjectBB();

			a_CurNode->translation = Vector3(0, 0, 0);
			if (a_AllNodes[a_CurNodeIndex]->GetObjectBB()["translation"])
			{
				BBE::JSONList list = a_AllNodes[a_CurNodeIndex]->GetObjectBB()["translation"]->GetListBB();
				a_CurNode->translation = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
			}

			//Assign mesh name
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
				a_GLTFFile->PrimitiveCount++;

				constexpr int NumOfAttibutes = 4;
				const char* attributes[NumOfAttibutes] = { {"POSITION"}, {"TEXCOORD_0"}, {"NORMAL"}, {"TANGENT"} };

				if (primitiveObj["attributes"])
				{
					//Note(Stan): Don't assume these are here..
					JSONObject& attributeObject = primitiveObj["attributes"]->GetObjectBB();

					for (size_t curAttibute = 0; curAttibute < NumOfAttibutes; curAttibute++)
					{
						a_CurNode->mesh.primative[primitiveIndex].countData[curAttibute] = ParseAttribute(&a_CurNode->mesh.primative[primitiveIndex].attributeData[curAttibute], attributeObject, attributes[curAttibute]);
					}

					a_GLTFFile->totalVertexCount += a_CurNode->mesh.primative[primitiveIndex].vertexCount;
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

						if (pbrMetallicRoughnessObj["baseColorTexture"])
						{
							uint32_t baseColorIndex = pbrMetallicRoughnessObj["baseColorTexture"]->GetObjectBB()["index"]->GetFloatBB();
							std::string str = m_CurImages[(uint32_t)m_CurTextures[baseColorIndex]->GetObjectBB()["source"]->GetFloatBB()]->GetObjectBB()["uri"]->GetStringBB();

							char* charPointer = (char*)malloc(str.size());
							strcpy(charPointer, str.c_str());
							a_CurNode->mesh.primative[primitiveIndex].Material.pbrMetallicRoughness.baseColorTexture.image.m_Path = charPointer;
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
		else {
			a_CurNode->ShouldRender = false;
		}

		if (a_AllNodes[a_CurNodeIndex]->GetObjectBB()["children"])
		{
			BBE::JSONList childNodes = a_AllNodes[a_CurNodeIndex]->GetObjectBB()["children"]->GetListBB();
			for (size_t childNodeIndex = 0; childNodeIndex < childNodes.size(); childNodeIndex++)
			{
				a_CurNodeIndex = childNodes[childNodeIndex]->GetFloatBB();
				BBE::Node* node = &a_GLTFFile->nodes[a_CurNodeIndex];
				CalculateNode(node, a_AllNodes, a_CurNodeIndex, a_GLTFFile);
			}
		}
	}

	//TODO(Stan):	Currently, I am passing a lot of values as reference that could be
	//				defined somewhere more global
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