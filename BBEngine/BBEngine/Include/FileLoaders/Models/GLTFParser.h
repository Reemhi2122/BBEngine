#include "System/FileHandler.h"
#include "Vector3.h"
#include "DataTypes/SharedRenderTypes.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	class GLTFParser {
	public:
		GLTFParser();
		GLTFParser(char* m_GLTFPath, char* a_GLTFName);
		~GLTFParser();

		GLTFFile* Parse(char* m_GLTFPath, char* a_GLTFName);
		void CalculateNode(BBE::Node* a_CurNode, BBE::JSONList& a_AllNodes, uint32_t a_CurNodeIndex, BBE::GLTFFile* a_GLTFFile);
		uint32_t ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, const char* a_Attribute);

	private:
		BBSystem::BBFILE m_BinFile;
	
		JsonParser m_Parser;
		GLTFFile m_GLTFFile;

		JSONList m_CurAccessorsList;
		JSONList m_CurBufferViews;
		JSONList m_CurMaterials;
		JSONList m_CurTextures;
		JSONList m_CurImages;
	};
}