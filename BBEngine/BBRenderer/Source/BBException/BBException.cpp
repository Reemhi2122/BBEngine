#include "BBException.h"

#include <dxerr.h>

//Note(Stan):   Some global exception code that is used througout the code base
static std::string ExTranslateErrorCode(HRESULT a_Hr) noexcept
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

/////////////////////////////
// Class specific exceptions
/////////////////////////////

//Windows exceptions
HrException::HrException(int a_Line, const char* a_File, HRESULT a_HR) noexcept
    : BBException(a_Line, a_File),
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
    return ExTranslateErrorCode(m_Hr);
}

const char* NoGfxException::GetType() const noexcept
{
    return "[BB Exception [No Graphics]]";
}


//Graphics exceptions
GfxHrException::GfxHrException(int a_Line, const char* a_File, HRESULT a_Hr) noexcept
	: BBException(a_Line, a_File),
	m_Hr(a_Hr)
{}

const char* GfxHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	oss << GetOriginString();

	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* GfxHrException::GetType() const noexcept
{
	return "[BB Graphics Exception]";
}

HRESULT GfxHrException::GetErrorCode() const noexcept
{
	return m_Hr;
}

std::string GfxHrException::GetErrorString() const noexcept
{
	return DXGetErrorString(m_Hr);
}

std::string GfxHrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(m_Hr, buf, sizeof(buf));
	return buf;
}

const char* DeviceRemovedException::GetType() const noexcept
{
	return "[BB Graphics Exception] [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}