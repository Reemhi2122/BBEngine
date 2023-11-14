#pragma once

class BBString {
public:
	BBString();
	BBString(const char* a_Char);
	~BBString();

private:
	char* m_String;

};