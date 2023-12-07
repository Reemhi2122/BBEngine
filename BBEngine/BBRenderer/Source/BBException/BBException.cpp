#include "BBException.h"

BBException::BBException(int a_Line, const char* a_File) noexcept
	: m_Line(a_Line), m_File(a_File)
{}

const char* BBException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType()
		<< std::endl
		<< GetOriginString()
		<< std::endl;

	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* BBException::GetType() const noexcept
{
	return "BBException";
}

int BBException::GetLine() const noexcept
{
	return m_Line;
}

const std::string& BBException::GetFile() const noexcept
{
	return m_File;
}

const std::string BBException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_File << std::endl
		<< "[Line]" << m_Line;

	return oss.str();
}

std::string Exception::TranslateErrorCode(HRESULT a_Hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, a_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

    if (nMsgLen == 0)
        return "Unidentified error code";

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HrException::HrException(int a_Line, const char* a_File, HRESULT a_HR) noexcept
    : Exception(a_Line, a_File),
    m_Hr(a_HR)
{}

const char* HrException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code]" << GetErrorCode() << std::endl
        << "[Descriptoin]" << GetErrorString() << std::endl
        << GetOriginString() << std::endl;
    m_WhatBuffer = oss.str();
    return m_WhatBuffer.c_str();
}

const char* HrException::GetType() const noexcept {
    return "[BBWindow Exception]";
}

HRESULT HrException::GetErrorCode() const noexcept {
    return m_Hr;
}

std::string HrException::GetErrorString() const noexcept {
    return TranslateErrorCode(m_Hr);
}

const char* NoGfxException::GetType() const noexcept
{
    return "[BB Exception [No Graphics]]";
}