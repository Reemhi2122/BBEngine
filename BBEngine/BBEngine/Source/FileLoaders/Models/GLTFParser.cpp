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

			JSONList& accessorsList = parser.GetRootNode()["accessors"]->GetListBB();
			JSONList& bufferViews = parser.GetRootNode()["bufferViews"]->GetListBB();

			//Go over all primitives
			BBE::JSONList& primitiveList = curMesh["primitives"]->GetListBB();
			for (uint32_t primitiveIndex = 0; primitiveIndex < primitiveList.size(); primitiveIndex++)
			{
				JSONObject& primitiveObj = primitiveList[primitiveIndex]->GetObjectBB();

				constexpr int NumOfAttibutes = 4;
				const char* attributes[NumOfAttibutes] = {{"POSITION"}, {"TEXCOORD_0"}, {"TANGENT"}, {"POSITION"}};

				if (primitiveObj.find("attributes") != primitiveObj.end())
				{
					//Note(Stan): Don't assume these are here..
					JSONObject& attributeObject = primitiveObj["attributes"]->GetObjectBB();

					for (size_t curAttibute = 0; curAttibute < NumOfAttibutes; curAttibute++)
					{
						gltfFile->nodes[i].mesh.counts.data[curAttibute] = ParseAttribute(&gltfFile->nodes[i].mesh.attributes.data[curAttibute], attributeObject, accessorsList, bufferViews, attributes[curAttibute]);
					}
				}

				gltfFile->nodes[i].mesh.indicesAmount = ParseAttribute(reinterpret_cast<void**>(&gltfFile->nodes[i].mesh.indices), primitiveObj, accessorsList, bufferViews, "indices");
			}

		}
		return gltfFile;
	}

	uint32_t GLTFParser::ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, JSONList& a_AccessorList, JSONList& a_BufferViews, const char* a_Attribute)
	{
		uint32_t accessorIndex = 0;
		uint32_t bufferViewIndex = 0;
		uint32_t bufferCount = 0;
		uint32_t byteLength = 0;
		uint32_t byteOffset = 0;

		if (a_AttributeObject[a_Attribute])
		{
			accessorIndex = static_cast<int>(a_AttributeObject[a_Attribute]->GetFloatBB());
			bufferViewIndex = static_cast<int>(a_AccessorList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
			bufferCount = static_cast<int>(a_AccessorList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());

			byteLength = static_cast<int>(a_BufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());

			if (a_BufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"])
			{
				byteOffset = static_cast<int>(a_BufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
			}

			*a_Data = malloc(bufferCount * (byteLength / bufferCount));
			BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(*a_Data), byteLength, byteOffset);
		}

		return bufferCount;
	}
}

//GLTFFile* GLTFParser::Parse(char* a_GLTFPath, char* a_GLTFName)
//{
//
//	GLTFFile* gltfFile = reinterpret_cast<GLTFFile*>(malloc(sizeof(GLTFFile)));
//
//	char GLTFPath[64] = "";
//	strcat(GLTFPath, a_GLTFPath);
//	strcat(GLTFPath, a_GLTFName);
//	JsonParser parser(GLTFPath);
//
//	char binPath[64] = "";
//	strcat(binPath, a_GLTFPath);
//	strcat(binPath, parser.GetRootNode()["buffers"]->GetListBB()[0]->GetObjectBB()["uri"]->GetStringBB().c_str());
//	m_BinFile = BBSystem::OpenFileReadBB(binPath);
//
//	//Currently only supports one scene
//
//	//Go through all nodes
//	BBE::JSONList& nodesList = parser.GetRootNode()["nodes"]->GetListBB();
//	gltfFile->nodes = reinterpret_cast<Node*>(malloc(nodesList.size() * sizeof(Node)));
//	//TODO(Stan):	Added -1 to this because there is a non node object
//	//				in the list. Look at this later.
//	for (size_t i = 0; i < nodesList.size() - 1; i++)
//	{
//		uint32_t curMeshIndex = nodesList[i]->GetObjectBB()["mesh"]->GetFloatBB();
//		BBE::JSONObject curMesh = parser.GetRootNode()["meshes"]->GetListBB()[curMeshIndex]->GetObjectBB();
//
//		BBE::JSONList list = nodesList[i]->GetObjectBB()["translation"]->GetListBB();
//		gltfFile->nodes[i].translation = Vector3(list[0]->GetFloatBB(), list[1]->GetFloatBB(), list[2]->GetFloatBB());
//
//		//Assign mesh name
//		//Note(Stan):	This only works when doing it indirectly,
//		//				Look into a way of fixing this.
//		std::string test = curMesh["name"]->GetStringBB();
//		gltfFile->nodes[i].mesh.name = test.c_str();
//
//		//Go over all primitives
//		BBE::JSONList& primitiveList = curMesh["primitives"]->GetListBB();
//		for (uint32_t primitiveIndex = 0; primitiveIndex < primitiveList.size(); primitiveIndex++)
//		{
//			JSONObject& primitiveObj = primitiveList[primitiveIndex]->GetObjectBB();
//			JSONList& accessorsList = parser.GetRootNode()["accessors"]->GetListBB();
//			JSONList& bufferViews = parser.GetRootNode()["bufferViews"]->GetListBB();
//
//			gltfFile->nodes[i].mesh.vertAmount = 0;
//
//			constexpr int NumOfAttibutes = 4;
//			const char* attributes[NumOfAttibutes] = { {"POSITION"}, {"TEXCOORD_0"}, {"TANGENT"}, {"POSITION"} };
//
//			uint32_t accessorIndex;
//			uint32_t bufferViewIndex;
//			uint32_t bufferCount;
//			uint32_t byteLength;
//			uint32_t byteOffset;
//
//			if (primitiveObj.find("attributes") != primitiveObj.end())
//			{
//				//Note(Stan): Don't assume these are here..
//				JSONObject& attributeObject = primitiveObj["attributes"]->GetObjectBB();
//
//				for (size_t curAttibute = 0; curAttibute < NumOfAttibutes; curAttibute++)
//				{
//					accessorIndex = 0;
//					bufferViewIndex = 0;
//					bufferCount = 0;
//					byteLength = 0;
//					byteOffset = 0;
//
//					if (attributeObject[attributes[curAttibute]])
//					{
//						accessorIndex = static_cast<int>(attributeObject[attributes[curAttibute]]->GetFloatBB());
//						bufferViewIndex = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
//						bufferCount = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());
//
//						byteLength = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());
//
//						if (bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"])
//						{
//							byteOffset = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
//						}
//
//						if (curAttibute == 1)
//						{
//							gltfFile->nodes[i].mesh.vertAmount = bufferCount;
//						}
//
//						gltfFile->nodes[i].mesh.attributes.data[curAttibute] = malloc(bufferCount * (byteLength / bufferCount));
//						BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(gltfFile->nodes[i].mesh.attributes.data[curAttibute]), byteLength, byteOffset);
//					}
//				}
//			}
//
//			//Indices
//			accessorIndex = static_cast<int>(primitiveObj["indices"]->GetFloatBB());
//			bufferViewIndex = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["bufferView"]->GetFloatBB());
//			bufferCount = static_cast<int>(accessorsList[accessorIndex]->GetObjectBB()["count"]->GetFloatBB());
//
//			byteLength = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteLength"]->GetFloatBB());
//			byteOffset = static_cast<int>(bufferViews[bufferViewIndex]->GetObjectBB()["byteOffset"]->GetFloatBB());
//
//			gltfFile->nodes[i].mesh.indicesAmount = bufferCount;
//			gltfFile->nodes[i].mesh.indices = reinterpret_cast<unsigned short*>(malloc(byteLength));
//			BBSystem::ReadFileAtBB(m_BinFile, reinterpret_cast<unsigned char*>(gltfFile->nodes[i].mesh.indices), byteLength, byteOffset);
//
//			//Process material
//		}
//
//	}
//	return gltfFile;
//}