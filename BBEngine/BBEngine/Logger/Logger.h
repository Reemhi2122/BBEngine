#pragma once
#include <string>

namespace BBlogger
{
	using ChannelHandle = uint32_t;

	using WarningTypeFlags = uint16_t;
	enum LogFlag : WarningTypeFlags {
		LogInfo = (1 << 0),
		LogWarningLow = (1 << 1),
		LogWarningMedium = (1 << 2),
		LogWarningHigh = (1 << 3),
		LogAssert = (1 << 4)
	};



	class Logger 
	{
	public:
		Logger();
		Logger(const Logger& rhs) = default;
		Logger& operator=(const Logger& rhs);
		~Logger() = default;

		static Logger* GetInstance();

		void SetupLogger(const std::string& loggerName, const std::string& loggerFileLocation = "", const LogFlag& loggerMinimumFlag = LogFlag::LogInfo);

		ChannelHandle RegisterChannel(const char* a_Name);

		void Log(const LogFlag& flag, const std::string& logMessage);
		void LogF(const LogFlag& flag, const std::string& logMessage, ...);


	private:
		std::string LogTagToText(const LogFlag& flag);
		void PrintToFile(const std::string& fileLocation, const std::string& message);

		struct Channel
		{
			const char* name;
			FILE* file;
			uint32_t flag;
			LogFlag m_MinimalFlag;
		};

		static Logger* instance;

		uint32_t m_NextFreeChannel = 0;
		Channel m_Channels[32];

		std::string m_LoggerName;
		std::string m_LoggerFileLocation;
		std::string m_LoggerFilePath;
		LogFlag m_MinimalFlag;


	};
}