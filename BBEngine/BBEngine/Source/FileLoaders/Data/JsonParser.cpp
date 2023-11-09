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

	void JSONNode::SetStringBB(std::string a_String)
	{
		if (type == NodeType::String) {
			value.string->assign(a_String);
			return;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	void JSONNode::SetFloatBB(float a_Float)
	{
		if (type == NodeType::Number) {
			value.floatValue = a_Float;
			return;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	void JSONNode::SetBoolBB(bool a_Bool)
	{
		if (type == NodeType::Boolean) {
			value.boolValue = a_Bool;
			return;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	JsonParser::JsonParser()
	{
		m_Root = nullptr;
		m_PrevPos = 0;
	}

	JsonParser::JsonParser(const char* a_FilePath)
	{
		Parse(a_FilePath);
	}

	JsonParser::~JsonParser()
	{
		m_JsonAlloc.Clear();
	}

	void JsonParser::Parse(const char* a_FilePath)
	{
		m_JsonAlloc.Init(1 * MBSize);
		m_FStream.LoadFile(a_FilePath);

		JSONToken tkn;
		tkn = GetToken();

		m_Root = SwitchOn(tkn.type);
	}

	void JsonParser::WriteJson(const char* a_FilePath)
	{
		BBSystem::BBFILE file = BBSystem::OpenFileWriteBB(a_FilePath);
		std::string out;

		JSONNode* node = m_List.Pop_Front();
		out = WriteJsonRecursive(node, "", false);

		BBSystem::WriteToFileBB(file, out);
	}

	std::string JsonParser::WriteJsonRecursive(JSONNode* a_Node, std::string a_Prefix, bool a_Last) {
		std::string out;
		switch (a_Node->type)
		{
		case NodeType::String:
		{
			out.append("\"" + a_Node->GetStringBB() + "\"");
			out.append(a_Last ? "," : "");
			out.append("\n");
			break;
		}
		case NodeType::Number:
		{
			out.append(std::to_string(a_Node->GetFloatBB()));
			out.append(a_Last ? "," : "");
			out.append("\n");
			break;
		}
		case NodeType::Boolean:
		{
			out.append(a_Node->GetBoolBB() ? "true" : "false");
			out.append(a_Last ? "," : "");
			out.append("\n");
			break;
		}
		case NodeType::Object:
		{
			out.append("{\n");
			uint32_t size = a_Node->GetObjectBB().size();
			for (size_t i = 0; i < size; i++) {
				JSONNode* node = m_List.Pop_Front();
				out.append(a_Prefix + "    ");
				out.append("\"" + node->GetStringBB() + "\"");
				out.append(": ");
				node = m_List.Pop_Front();
				
				bool last = i != (size - 1);
				out.append(WriteJsonRecursive(node, "    ", last));
			}
			out.append(a_Prefix);
			out.append("}");
			out.append(a_Last ? "," : "");
			out.append("\n");
			break;
		}
		case NodeType::List:
		{
			out.append("[\n");
			uint32_t size = a_Node->GetListBB().size();
			for (size_t i = 0; i < size; i++) {
				out.append(a_Prefix + "    ");
				JSONNode* node = m_List.Pop_Front();
				bool last = i != (size - 1);
				out.append(WriteJsonRecursive(node, a_Prefix + "    ", last));
			}
			out.append(a_Prefix);
			out.append("]");
			out.append(a_Last ? "," : "");
			out.append("\n");
			break;
		}
		default:
			break;
		}

		return out;
	}

	JSONNode* JsonParser::ParseObject()
	{
		JSONNode* node = BBNew(m_JsonAlloc, JSONNode);
		JSONObject* obj = BBNew(m_JsonAlloc, JSONObject);
		bool hasCompleted = false;
		m_List.Push_Back(node);

		while (!hasCompleted) {
			if (!EndOfFile()) {
				JSONToken curtkn = GetToken();
				std::string key = curtkn.value;
				JSONNode* keynode = SwitchOn(curtkn.type);

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
		JSONNode* node = BBNew(m_JsonAlloc, JSONNode);
		JSONList* list = BBNew(m_JsonAlloc, JSONList);
		bool hasCompleted = false;
		m_List.Push_Back(node);

		while (!hasCompleted) {
			if (!EndOfFile()) {
				JSONToken curtkn = GetToken();
				JSONNode* listNode = SwitchOn(curtkn.type);
				
				list->push_back(listNode);
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
		JSONNode* node = BBNew(m_JsonAlloc, JSONNode);
		JSONToken tkn = GetToken();
	
		node->value.string = BBNew(m_JsonAlloc, std::string)(tkn.value.c_str());
		node->type = NodeType::String;
		m_List.Push_Back(node);
		return node;
	}

	JSONNode* JsonParser::ParseNumber()
	{
		JSONNode* node = BBNew(m_JsonAlloc, JSONNode);
		JSONToken tkn = GetToken();

		node->value.floatValue = std::stof(tkn.value);
		node->type = NodeType::Number;
		m_List.Push_Back(node);
		return node;
	}

	JSONNode* JsonParser::ParseBool()
	{
		JSONNode* node = BBNew(m_JsonAlloc, JSONNode);
		JSONToken tkn = GetToken();

		node->value.boolValue = strcmp(tkn.value.c_str(), "f");
		node->type = NodeType::Boolean;
		m_List.Push_Back(node);
		return node;
	}

	JSONNode* JsonParser::ParseNull()
	{
		JSONNode* node = BBNew(m_JsonAlloc, JSONNode);
		node->type = NodeType::NULL_Type;
		m_List.Push_Back(node);
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
		m_PrevPos = m_FStream.GetPos();
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
		else if (c == '-' || c >= '0' && c <= '9') {
			tkn.type = JSONTokenType::Number;
			tkn.value = "";
			tkn.value += c;

			uint32_t prevCharPos;
			while (c == '-' || c == '.' || c >= '0' && c <= '9') {
				prevCharPos = m_FStream.GetPos();
				m_FStream.Get(c);

				if (m_FStream.Eof()) {
					break;
				}
				
				if (c == '-' || c == '.' || c >= '0' && c <= '9') {
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
		while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')) {

			m_FStream.Get(c);

			if ((c == ' ' || c == '\n' || c == '\t' || c == '\r') && !m_FStream.Good()) {
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
		m_FStream.SeekPos(m_PrevPos);
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