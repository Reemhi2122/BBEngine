#pragma once
#include <string>
#include <map>
#include <vector>
#include "System/BBFStream.h"

namespace BBE {

	struct JSONNode;

	//NOTE(Stan): might remove these later for own datastructures
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
	};

	class JsonParser {
	public:
		JsonParser() = default;
		JsonParser(const char* a_FilePath);
		~JsonParser() = default;

		void Parse(const char* a_FilePath);
		JSONObject GetRootNode() const noexcept;

	private:
		JSONToken GetToken();
		char GetWithoutWhiteSpace();
		void RollBackToken();
		bool EndOfFile();

		JSONNode* ParseObject();
		JSONNode* ParseList();
		JSONNode* ParseString();
		JSONNode* ParseNumber();
		JSONNode* ParseBool();
		JSONNode* ParseNull();

		JSONNode* SwitchOn(JSONTokenType& a_Type);

		BBSystem::BBFStream m_FStream;
		size_t prevPos;

		JSONNode* root;
		JSONNode* current;
	};

	inline JSONObject JsonParser::GetRootNode() const noexcept {
		return root->GetObjectBB();
	}

}