#pragma once
#include <string>
#include <map>
#include <vector>
#include "System/BBFStream.h"
#include "Containers/LinkedList.h"

namespace BBE {

	struct JSONNode;

	//NOTE(Stan): might remove these later for own datastructures
	//NOTE(Stan): Replaced vector with my own linked list, might move it back to my own vector
	using JSONObject = std::map<std::string, JSONNode*>;
	using JSONList = std::vector<JSONNode*>;

	enum class NodeType { 
		Object, 
		List, 
		String, 
		Number, 
		Boolean, 
		NULL_Type 
	};

	enum class JSONTokenType {
		CurlyOpen,
		CurlyClose,
		Colon,
		String,
		Number,
		ArrayOpen,
		ArrayClose,
		Comma,
		Boolean,
		NullType
	};

	struct JSONToken {
		JSONTokenType type;
		std::string value;
	};

	class JSONNode {
	public:
		union Values {
			JSONObject* obj;
			JSONList* list;
			std::string* string;
			float floatValue;
			bool boolValue;
		} value;
		NodeType type;
	
		JSONObject GetObjectBB() const;
		JSONList GetListBB() const;
		std::string GetStringBB() const;
		float GetFloatBB() const;
		bool GetBoolBB() const;

		void SetStringBB(std::string a_String);
		void SetFloatBB(float a_Float);
		void SetBoolBB(bool a_Bool);
	};

	class JsonParser {
	public:
		JsonParser();
		JsonParser(const char* a_FilePath);
		~JsonParser();

		void Parse(const char* a_FilePath);
		void WriteJson(const char* a_FilePath);
		JSONObject GetRootNode() const noexcept;

	private:
		JSONToken GetToken();
		char GetWithoutWhiteSpace();
		void RollBackToken();
		bool EndOfFile();

		std::string WriteJsonRecursive(JSONNode* node, std::string a_Prefix, bool a_Last);

		JSONNode* ParseObject();
		JSONNode* ParseList();
		JSONNode* ParseString();
		JSONNode* ParseNumber();
		JSONNode* ParseBool();
		JSONNode* ParseNull();

		JSONNode* SwitchOn(JSONTokenType& a_Type);

		Allocators::StackAllocator m_JsonAlloc;
		BBSystem::BBFStream m_FStream;
		size_t m_PrevPos;
		JSONNode* m_Root;
		LinkedList<JSONNode*> m_List;
	};

	inline JSONObject JsonParser::GetRootNode() const noexcept {
		return m_Root->GetObjectBB();
	}
}