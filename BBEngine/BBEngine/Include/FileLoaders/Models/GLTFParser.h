#include "System/FileHandler.h"
#include "Vector3.h"
#include "DataTypes/SharedRenderTypes.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	class GLTFParser {
	public:
		GLTFParser();
		~GLTFParser();

		bool Parse(char* m_GLTFPath, char* a_GLTFName, GLTFFile* a_GLTFFile);
		void CalculateNode(BBE::Node* a_CurNode, uint32_t a_CurNodeIndex);
		uint32_t ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, const char* a_Attribute, uint8_t* a_DataSize = NULL);

	private:
		BBSystem::BBFILE m_BinFile;
	
		JsonParser m_Parser;
		GLTFFile* m_GLTFFile;

		JSONList m_AllNodes;
		JSONList m_CurAccessorsList;
		JSONList m_CurBufferViews;
		JSONList m_CurMaterials;
		JSONList m_CurTextures;
		JSONList m_CurImages;
	};
}