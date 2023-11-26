#pragma once

namespace BBE {
	
	template<typename TypeChar>
	class BB_Basic_String {
	public:
		BB_Basic_String();
		BB_Basic_String(TypeChar* a_Str);
		~BB_Basic_String();

		BB_Basic_String<TypeChar>* Append(const BB_Basic_String& a_Str);

		BB_Basic_String<TypeChar>* CStr();

	private:
		TypeChar* m_String;
	};

	typedef BB_Basic_String<char> BBString;
	typedef BB_Basic_String<wchar_t> BBWString;

	template<typename TypeChar>
	BB_Basic_String<TypeChar>::BB_Basic_String(){
		m_String = nullptr;
	}

	template<typename TypeChar>
	BB_Basic_String<TypeChar>::BB_Basic_String(TypeChar* a_Str){
		m_String = a_Str;
	}

	template<typename TypeChar>
	BB_Basic_String<TypeChar>::~BB_Basic_String() {

	}

	template<typename TypeChar>
	BB_Basic_String<TypeChar>* BB_Basic_String<TypeChar>::Append(const BB_Basic_String& a_Str) {

		return this;
	}

	template<typename TypeChar>
	BB_Basic_String<TypeChar>* BB_Basic_String<TypeChar>::CStr() {
		return m_String;
	}

}