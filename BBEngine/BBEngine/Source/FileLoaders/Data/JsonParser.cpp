#include "FileLoaders/Data/JsonParser.h"

#include "Logger/Logger.h"

namespace BBE {

	JSONObject JSONNode::GetObject() const {
		if (type == NodeType::Object) {
			return *value.obj;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	JSONList JSONNode::GetList() const
	{
		if (type == NodeType::List) {
			return *value.list;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	std::string JSONNode::GetString() const
	{
		if (type == NodeType::String) {
			return *value.string;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	float JSONNode::GetFloat() const 
	{
		if (type == NodeType::Number) {
			return value.floatValue;
		}

		BB_Assert(0, "Requested JSON node not of correct type!");
	}

	bool JSONNode::GetBool() const
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

		char c;
		while (m_FStream.Good()) {

			m_FStream.Get(c);
			printf("%c", c);
		}
	}

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
		else if (c == '-' || c > '1' && c < '9') {
			tkn.type = JSONTokenType::Number;
			tkn.value = "";
			tkn.value += c;

			uint32_t prevCharPos;
			while (c == '-' || c == '.' || c > '1' && c < '9') {
				prevCharPos = m_FStream.GetPos();
				m_FStream.Get(c);

				if (m_FStream.Eof()) {
					break;
				}
				
				if (c == '-' || c == '.' || c > '1' && c < '9') {
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
		while ((c == ' ' || c == '\n')) {

			m_FStream.Get(c);

			if ((c == ' ' || c == '\n') && !m_FStream.Good()) {
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
}