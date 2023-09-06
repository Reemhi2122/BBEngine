#include "Logger.h"
#include <fstream>
#include <stdarg.h>

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

	void Logger::Log(const LogFlag& flag, const std::string& logMessage)
	{
		if(flag < m_MinimalFlag)
		{
			return;
		}

		std::string output = LogTagToText(flag);
		output.append(logMessage);
		output.append("\n");

		printf(output.c_str());
		PrintToFile("", output);

	}

	void Logger::LogF(const LogFlag& flag, const std::string& logMessage, ...)
	{
		va_list va_format_list;
		va_start(va_format_list, &logMessage);

		std::string output = LogTagToText(flag);
		output.append(logMessage);
		output.append("\n");

		vprintf(output.c_str(), va_format_list);
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

	std::string Logger::LogTagToText(const LogFlag& flag)
	{
		std::string output;
		switch (flag)
		{
		case LogFlag::LogInfo:
			output = "[Info] ";
			break;
		case LogFlag::LogWarningLow:
			output = "[Warning Low] ";
			break;
		case LogFlag::LogWarningMedium:
			output = "[Warning Medium] ";
			break;
		case LogFlag::LogWarningHigh:
			output = "[Warning High] ";
			break;
		case LogFlag::LogAssert:
			output = "[Warning High] ";
			break;
		default:
			break;
		}
		return output;
	}

	void Logger::PrintToFile(const std::string& fileLocation, const std::string& message)
	{
		std::ofstream file;

		file.open(m_LoggerFilePath);
		file << message;
		file.close();
	}
}

BBlogger::Logger* BBlogger::Logger::instance = nullptr;