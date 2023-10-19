#include "FileLoaders/Data/JsonParser.h"

#include "Logger/Logger.h"

namespace BBE {

	JSONObject JSONNode::GetObjectBB() const {
		if (type == NodeType::Object) {
			return *value.obj;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	JSONList JSONNode::GetListBB() const
	{
		if (type == NodeType::List) {
			return *value.list;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	std::string JSONNode::GetStringBB() const
	{
		if (type == NodeType::String) {
			return *value.string;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	float JSONNode::GetFloatBB() const
	{
		if (type == NodeType::Number) {
			return value.floatValue;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	bool JSONNode::GetBoolBB() const
	{
		if (type == NodeType::Boolean) {
			return value.boolValue;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	JsonParser::JsonParser(const char* a_FilePath)
	{
		Parse(a_FilePath);
	}

	void JsonParser::Parse(const char* a_FilePath)
	{
		m_FStream.LoadFile(a_FilePath);

		JSONToken tkn;
		tkn = GetToken();
		BB_Log(DEFAULT_LOG_CHANNEL, BBUtility::LogInfo, tkn.value.c_str());

		root = SwitchOn(tkn.type);
	}

	JSONNode* JsonParser::ParseObject()
	{
		//NOTE(Stan): Need to change this to memory allocator
		JSONNode* node;
		JSONObject* obj = new JSONObject();
		bool hasCompleted = false;

		while (!hasCompleted) {
			if (!EndOfFile()) {
				node = new JSONNode();
				JSONToken curtkn = GetToken();
				std::string key = curtkn.value;
				GetToken();
				curtkn = GetToken();

				(*obj)[key] = SwitchOn(curtkn.type);

				curtkn = GetToken();
				hasCompleted = (curtkn.type == JSONTokenType::CurlyClose);
			}
			else {
				BB_Assert(0, "No more tokens left while creating list.");
			}
		}

		node->value.obj = obj;
		node->type = NodeType::Object;
		return node;
	}

	JSONNode* JsonParser::ParseList()
	{
		//NOTE(Stan): Need to change this to memallocator
		
		JSONList* list = new JSONList();
		JSONNode* node;
		bool hasCompleted = false;

		while (!hasCompleted) {
			if (!EndOfFile()) {
				JSONToken curtkn = GetToken();

				node = SwitchOn(curtkn.type);
				
				list->push_back(node);
				curtkn = GetToken();
				hasCompleted = (curtkn.type == JSONTokenType::ArrayClose);
			}
			else {
				BB_Assert(0, "No more tokens left while creating list.");
			}
		}

		node->value.list = list;
		node->type = NodeType::List;
		return node;
	}

	JSONNode* JsonParser::ParseString()
	{
		JSONNode* node = new JSONNode();
		JSONToken tkn = GetToken();
	
		node->value.string = new std::string(tkn.value.c_str());
		node->type = NodeType::String;
		return node;
	}

	JSONNode* JsonParser::ParseNumber()
	{
		JSONNode* node = new JSONNode();
		JSONToken tkn = GetToken();

		node->value.floatValue = std::stof(tkn.value);
		node->type = NodeType::Number;
		return node;
	}

	JSONNode* JsonParser::ParseBool()
	{
		JSONNode* node = new JSONNode();
		JSONToken tkn = GetToken();

		node->value.floatValue = strcmp(tkn.value.c_str(), "f");
		node->type = NodeType::Number;
		return node;
	}

	JSONNode* JsonParser::ParseNull()
	{
		JSONNode* node = new JSONNode();
		node->type = NodeType::NULL_Type;
		return node;
	}

	//NOTE(Stan): Might want to do some more checking on the 
	//True, False & Null if they are actually spelled correctly
	JSONToken JsonParser::GetToken()
	{
		char c;
		if (m_FStream.Eof()) {
			BB_Log(DEFAULT_LOG_CHANNEL, BBUtility::LogInfo, "Exhaused tokens");
		}
		prevPos = m_FStream.GetPos();
		c = GetWithoutWhiteSpace();

		JSONToken tkn;
		if (c == '"') {
			tkn.type = JSONTokenType::String;
			tkn.value = "";
			
			m_FStream.Get(c);
			while (c != '"') {
				tkn.value += c;
				m_FStream.Get(c);
			}
		}
		else if (c == '-' || c >= '1' && c <= '9') {
			tkn.type = JSONTokenType::Number;
			tkn.value = "";
			tkn.value += c;

			uint32_t prevCharPos;
			while (c == '-' || c == '.' || c >= '1' && c <= '9') {
				prevCharPos = m_FStream.GetPos();
				m_FStream.Get(c);

				if (m_FStream.Eof()) {
					break;
				}
				
				if (c == '-' || c == '.' || c >= '1' && c <= '9') {
					tkn.value += c;
				}
				else {
					m_FStream.SeekPos(prevCharPos);
				}
			}
		}
		else {
			uint8_t charNum = static_cast<uint8_t>(c);
			switch (charNum)
			{
			case '{':
				tkn.type = JSONTokenType::CurlyOpen;
				break;
			case '}':
				tkn.type = JSONTokenType::CurlyClose;
				break;
			case '[':
				tkn.type = JSONTokenType::ArrayOpen;
				break;
			case ']':
				tkn.type = JSONTokenType::ArrayClose;
				break;
			case ':':
				tkn.type = JSONTokenType::Colon;
				break;
			case ',':
				tkn.type = JSONTokenType::Comma;
				break;
			case 't':
				tkn.type = JSONTokenType::Boolean;
				tkn.value = "True";
				m_FStream.SeekPos(m_FStream.GetPos() + 3);
				break;
			case 'f':
				tkn.type = JSONTokenType::Boolean;
				tkn.value = "False";
				m_FStream.SeekPos(m_FStream.GetPos() + 4);
				break;
			case 'n':
				tkn.type = JSONTokenType::NullType;
				m_FStream.SeekPos(m_FStream.GetPos() + 3);
				break;
			default:
				break;
			}
		}

		return tkn;
	}
	
	char JsonParser::GetWithoutWhiteSpace()
	{
		char c = ' ';
		while ((c == ' ' || c == '\n' || c == '\t')) {

			m_FStream.Get(c);

			if ((c == ' ' || c == '\n' || c == '\t') && !m_FStream.Good()) {
				BB_Assert(0, "Ran out of tokens during parsing!");
			}
			else if (!m_FStream.Good()) {
				return c;
			}
		}

		return c;
	}

	void JsonParser::RollBackToken()
	{
		if (m_FStream.Eof()) {
			m_FStream.Clear();
		}
		m_FStream.SeekPos(prevPos);
	}

	bool JsonParser::EndOfFile()
	{
		return m_FStream.Eof();
	}

	JSONNode* JsonParser::SwitchOn(JSONTokenType& a_Type)
	{
		switch (a_Type)
		{
		case JSONTokenType::CurlyOpen:
			return ParseObject();
		case JSONTokenType::ArrayOpen:
			return ParseList();
		case JSONTokenType::String:
			RollBackToken();
			return ParseString();
		case JSONTokenType::Number:
			RollBackToken();
			return ParseNumber();
		case JSONTokenType::Boolean:
			RollBackToken();
			return ParseBool();
		case JSONTokenType::NullType:
			return ParseNull();
		default:
			break;
		}
	}
}