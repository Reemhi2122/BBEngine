#pragma once
#include <exception>
#include <string>
#include <sstream>

#include "BBWin.h"

class BBException : public std::exception
{
public:
	BBException(int line, const char* m_File) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	const std::string GetOriginString() const noexcept;

private:
	int m_Line;
	std::string m_File;

protected:
	mutable std::string m_WhatBuffer;
};

class Exception : public BBException {
public:
	using BBException::BBException;
	static std::string TranslateErrorCode(HRESULT a_Hr) noexcept;
};

class HrException : public Exception {
public:
	HrException(int a_Line, const char* a_File, HRESULT a_HR) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;

private:
	HRESULT m_Hr;
};

class NoGfxException : public Exception {
public:
	using Exception::Exception;
	const char* GetType() const noexcept override;
};