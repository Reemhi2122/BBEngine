#pragma once
#include <exception>
#include <string>
#include <sstream>

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