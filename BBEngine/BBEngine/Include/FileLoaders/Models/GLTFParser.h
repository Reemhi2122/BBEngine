#include "System/FileHandler.h"
#include "Vector3.h"
#include "SharedRenderTypes.h"
#include "FileLoaders/Data/JsonParser.h"

namespace BBE {

	constexpr uint8_t FILE_PATH_SIZE = 255;

	class GLTFParser {
	public:
		GLTFParser();
		~GLTFParser();

		bool Parse(const char* m_GLTFPath, const char* a_GLTFName, GLTFFile* a_GLTFFile);
		void CalculateNode(BBE::GLTFNode* a_CurNode, uint32_t a_CurNodeIndex);

	private:
		uint32_t ParseAttribute(void** a_Data, JSONObject& a_AttributeObject, const char* a_Attribute, uint8_t* a_DataSize = NULL);
		uint32_t ParseTexture(JSONObject a_CurObject, const char* a_TextureName, const char* a_AdditionalValueName, TextureT* a_TexureOut, float* a_OutAditionalValue);

		BBSystem::BBFHANDLE m_BinFile;
	
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