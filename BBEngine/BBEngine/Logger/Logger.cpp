#include "Logger/Logger.h"
#include <stdarg.h>
#include <cmath>
#include "Utility/HelperFunctions.h"
#include "System/FileHandler.h"

namespace BBE {
	namespace BBUtility
	{
		Logger::Logger()
		{
			Logger::instance = this;
			m_LogFilter = LogFlag::LogInfo;

			m_NextFreeChannel = 1;
			m_Channels[0].name = "Default";
			m_FileHandle = NULL;
		}

		Logger& Logger::operator=(const Logger& rhs)
		{
			if (this == &rhs)
				return *this;

			m_LoggerName = rhs.m_LoggerName;
			m_DirectoryPath = rhs.m_DirectoryPath;
			m_FilePath = rhs.m_FilePath;
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

		void Logger::PrintToFile(const std::string& a_Message)
		{
			if (BBSystem::FileExistsBB(m_DirectoryPath)) {
				BB_Assert(0, "Logger - No file found");
			}

			BBSystem::BBFILE file = BBSystem::OpenFileBB(m_FilePath);
			BBSystem::WriteToFileBB(file, a_Message);
			BBSystem::CloseFileBB(file);
		}

		void Logger::Log(const ChannelHandle a_Handle, const LogFlag& a_Flag, const std::string& a_LogMessage, const char* a_File, const int& a_Line)
		{
			if (!ValidFilter(a_Handle, a_Flag))
				return;

			std::string message = FormatLogMessage(a_Flag, a_Handle, a_LogMessage, a_File, a_Line);

			m_LoggerThread = std::thread(&Logger::PrintToFile, this, message);
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
			m_DirectoryPath = loggerFileLocation;
			m_LogFilter = loggerMinimumFlag;

			m_FilePath.clear();
			m_FilePath = m_DirectoryPath;
			m_FilePath.append(loggerName);
			m_FilePath.append(".txt");

			BBSystem::CreateDirBB(m_DirectoryPath);
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
}

BBE::BBUtility::Logger* BBE::BBUtility::Logger::instance = nullptr;