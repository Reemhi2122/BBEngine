#pragma once
#include <exception>
#include <string>
#include <sstream>

//TODO(Stan):	Don't really want this call to BBWin in here
//				need to look into reworking the exception class
#include "BBWin.h"

//Base class exceptions
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

//TODO(Stan):	Need to rework the exception system to work more together

/////////////////////////////
// Class specific exceptions
/////////////////////////////

//Window exceptions
class HrException : public BBException {
public:
	HrException(int a_Line, const char* a_File, HRESULT a_HR) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;

private:
	HRESULT m_Hr;
};

class NoGfxException : public BBException {
public:
	using BBException::BBException;
	const char* GetType() const noexcept override;
};

//Graphics exceptions
class GfxHrException : public BBException {
public:
	GfxHrException(int a_Line, const char* a_File, HRESULT a_Hr) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;
	std::string GetErrorDescription() const noexcept;

private:
	HRESULT m_Hr;
};

class DeviceRemovedException : public GfxHrException {
public:
	GfxHrException::GfxHrException;
	const char* GetType() const noexcept override;
};