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