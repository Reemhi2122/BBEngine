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


		return JSONToken();
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

	}
}