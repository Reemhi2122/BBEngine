#include "Logger.h"
#include <fstream>
#include <stdarg.h>
#include <cmath>
#include "../Utility/HelperFunctions.h"

namespace BBUtility
{
	Logger::Logger()
	{
		Logger::instance = this;
		m_LogFilter = LogFlag::LogInfo;

		m_NextFreeChannel = 1;
		m_Channels[0].name = "Default";
	}

	Logger& Logger::operator=(const Logger& rhs)
	{
		if (this == &rhs)
			return *this;

		m_LoggerName = rhs.m_LoggerName;
		m_LoggerFileLocation = rhs.m_LoggerFileLocation;
		m_LoggerFilePath = rhs.m_LoggerFilePath;
		m_LogFilter = rhs.m_LogFilter;
		m_NextFreeChannel = rhs.m_NextFreeChannel;

		for (size_t i = 0; i < MAXCHANNELAMOUNT; i++) {
			m_Channels[i] = rhs.m_Channels[i];
		}

		return *this;
	}

	Logger::~Logger()
	{
	}

	Logger* Logger::GetInstance()
	{
		if (instance == nullptr)
			instance = new Logger();

		return instance;
	}

	void Logger::PrintToFile(const std::string& a_Message, const std::string& a_Path)
	{
		std::ofstream file;

		file.open(a_Path, std::ofstream::app);
		file << a_Message;
		file.close();
	}

	void Logger::Log(const ChannelHandle a_Handle, const LogFlag& a_Flag, const std::string& a_LogMessage, const char* a_File, const int& a_Line)
	{
		if (!ValidFilter(a_Handle, a_Flag))
			return;

		std::string message = FormatLogMessage(a_Flag, a_Handle, a_LogMessage, a_File, a_Line);

		m_LoggerThread = std::thread(&Logger::PrintToFile, this, message, m_LoggerFilePath);
		printf(message.c_str());
		m_LoggerThread.join();
	}

	void Logger::LogF(const ChannelHandle a_Handle, const LogFlag& a_Flag, const std::string& a_LogMessage, const char* a_File, const int& a_Line, ...)
	{
		if (!ValidFilter(a_Handle, a_Flag))
			return;

		va_list va_format_list;
		va_start(va_format_list, &a_LogMessage);

		std::string message = FormatLogMessage(a_Flag, a_Handle, a_LogMessage, a_File, a_Line);

		vprintf(message.c_str(), va_format_list);
	}

	bool Logger::ValidFilter(const ChannelHandle& a_Handle, const LogFlag& a_Flag)
	{
		if (m_LogFilter & a_Flag && m_Channels[a_Handle].flagFilter & a_Flag) {
			return true;
		}	
		return false;
	}

	std::string Logger::FormatLogMessage(const LogFlag& a_LogFlag, const ChannelHandle& a_ChannelHandle, const std::string& a_LogMessage, const char* a_File, const int& a_Line)
	{
		int logFlagIndex = (std::log2((float)a_LogFlag));
		return BBUtility::string_format("\n[%s] - [%s] | [Line %d] at [%s]: \n%s \n", 
			m_Channels[a_ChannelHandle].name.c_str(), 
			LogFlagNames[logFlagIndex], 
			a_Line,
			a_File,
			a_LogMessage.c_str());
	}

	void Logger::SetupLogger(const std::string& loggerName, const WarningTypeFlags& loggerMinimumFlag, const std::string& loggerFileLocation) {
		m_LoggerName = loggerName;
		m_LoggerFileLocation = loggerFileLocation;
		m_LogFilter = loggerMinimumFlag;

		m_LoggerFilePath.clear();
		m_LoggerFilePath = m_LoggerFileLocation;
		m_LoggerFilePath.append(loggerName);
		m_LoggerFilePath.append(".txt");
	}

	void Logger::RegisterChannel(const char* a_Name, ChannelHandle& handle)
	{
		ChannelHandle curChannelHandle = m_NextFreeChannel;

		Channel curChannel = Channel();
		curChannel.name = a_Name;
		curChannel.flagFilter = LOG_ALL;
		m_Channels[curChannelHandle] = curChannel;

		m_NextFreeChannel++;
		handle = curChannelHandle;
	}
}

BBUtility::Logger* BBUtility::Logger::instance = nullptr;