#include "Logger.h"
#include <fstream>
#include <stdarg.h>
#include <cmath>
#include "../Helper/Utility.h"

namespace BBlogger
{
	Logger::Logger()
	{
		Logger::instance = this;
		m_MinimalFlag = LogFlag::LogInfo;
		
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
		m_MinimalFlag = rhs.m_MinimalFlag;
		m_NextFreeChannel = rhs.m_NextFreeChannel;
		
		for (size_t i = 0; i < MAXCHANNELAMOUNT; i++) {
			m_Channels[i] = rhs.m_Channels[i];
		}

		return *this;
	}
	
	Logger* Logger::GetInstance() 
	{
		if (instance == nullptr)
		{
			instance = new Logger();
		}

		return instance;
	}

	void Logger::Log(const ChannelHandle a_Handle, const LogFlag& a_Flag, const std::string& a_LogMessage)
	{
		if(a_Flag < m_MinimalFlag)
		{
			return;
		}

		std::string message = FormatLogMessage(a_Flag, a_Handle, a_LogMessage);
		printf(message.c_str());
		PrintToFile(message);
	}

	void Logger::LogF(const ChannelHandle a_Handle, const LogFlag& a_Flag, const std::string& a_LogMessage, ...)
	{
		va_list va_format_list;
		va_start(va_format_list, &a_LogMessage);

		std::string message = FormatLogMessage(a_Flag, a_Handle, a_LogMessage);
		vprintf(message.c_str(), va_format_list);
		PrintToFile(BBUtility::string_format(message.c_str(), va_format_list));
	}

	std::string Logger::FormatLogMessage(const LogFlag& a_LogFlag, const ChannelHandle& a_ChannelHandle, const std::string& a_LogMessage)
	{
		int logFlagIndex = std::floor((std::sqrt((float)a_LogFlag)));
		return BBUtility::string_format("[%s] - [%s]: %s \n", m_Channels[a_ChannelHandle].name.c_str(), LogFlagNames[logFlagIndex], a_LogMessage.c_str());
	}

	void Logger::SetupLogger(const std::string& loggerName, const std::string& loggerFileLocation, const LogFlag& loggerMinimumFlag) {
		m_LoggerName = loggerName;
		m_LoggerFileLocation = loggerFileLocation;
		m_MinimalFlag = loggerMinimumFlag;

		m_LoggerFilePath.clear();
		m_LoggerFilePath = m_LoggerFileLocation;
		m_LoggerFilePath.append(loggerName);
		m_LoggerFilePath.append(".txt");
	}

	ChannelHandle Logger::RegisterChannel(const char* a_Name)
	{
		ChannelHandle curChannelHandle = m_NextFreeChannel;
		
		Channel curChannel = Channel();
		curChannel.name = a_Name;
		m_Channels[curChannelHandle] = curChannel;

		m_NextFreeChannel++;
		return curChannelHandle;
	}

	void Logger::PrintToFile(const std::string& message)
	{
		std::ofstream file;

		file.open(m_LoggerFilePath, std::ofstream::app);
		file << message;
		file.close();
	}
}

BBlogger::Logger* BBlogger::Logger::instance = nullptr;