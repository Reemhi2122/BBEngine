#pragma once
#include <string>

namespace BBlogger
{
	using ChannelHandle = uint32_t;

	using WarningTypeFlags = uint8_t;
	enum LogFlag : WarningTypeFlags {
		LogInfo = (1 << 0),
		LogWarningLow = (1 << 1),
		LogWarningMedium = (1 << 2),
		LogWarningHigh = (1 << 3),
		LogAssert = (1 << 4)
	};

	constexpr int MAXCHANNELAMOUNT = 32;

	class Logger 
	{
	public:
		Logger();
		Logger(const Logger& rhs) = default;
		Logger& operator=(const Logger& rhs);
		~Logger() = default;
		static Logger* GetInstance();

		char LogFlagNames[5][32]
		{
			"Info",
			"Warning Low",
			"Warning Medium",
			"Warning High",
			"Assert"
		};

		void SetupLogger(const std::string& loggerName, const WarningTypeFlags& loggerMinimumFlag = LogFlag::LogInfo, const std::string& loggerFileLocation = "");
		ChannelHandle RegisterChannel(const char* a_Name);

		void Log(const ChannelHandle a_Handle, const LogFlag& flag, const std::string& logMessage);
		void LogF(const ChannelHandle a_Handle, const LogFlag& flag, const std::string& logMessage, ...);

	private:
		bool ValidFilter(const ChannelHandle& a_Handle, const LogFlag& a_Flag);
		std::string FormatLogMessage(const LogFlag& a_LogFlag, const ChannelHandle& a_ChannelHandle, const std::string& a_LogMessage);
		void PrintToFile(const std::string& message);

		struct Channel
		{
			std::string name;
			FILE* file;
			uint32_t flag;
			WarningTypeFlags flagFilter;
		};

		static Logger* instance;

		uint32_t m_NextFreeChannel = 0;
		Channel m_Channels[MAXCHANNELAMOUNT];
		WarningTypeFlags m_LogFilter;

		std::string m_LoggerName;
		std::string m_LoggerFileLocation;
		std::string m_LoggerFilePath;
	};
}