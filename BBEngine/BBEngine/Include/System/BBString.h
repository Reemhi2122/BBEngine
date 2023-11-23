#pragma once

template<typename TChar>
class BB_Basic_String {
public:
	BB_Basic_String();
	BB_Basic_String(const char* a_Char);
	~BB_Basic_String();

	void Append();

private:
	char* m_String;

};

typedef BB_Basic_String<wchar_t> BBString;
typedef BB_Basic_String<char> BBWString;
