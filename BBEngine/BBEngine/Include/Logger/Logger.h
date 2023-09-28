#pragma once
#include <string>
#include <thread>
#include <cassert>

#define DEFAULT_LOG_CHANNEL 0
#define LOG_ALL 127

#define BB_Log_Init(name, filter, path)																							\
do {																															\
	BBE::BBUtility::Logger::GetInstance()->SetupLogger(name, filter, path);														\
} while (0)

#define BB_RegisterChannel(name, handle)																						\
do{																																\
	BBE::BBUtility::Logger::GetInstance()->RegisterChannel(name, handle);														\
} while (0)

#define BB_Log(handle, severity, message)																						\
do{																																\
	BBE::BBUtility::Logger::GetInstance()->Log(handle, severity, message, __FILE__, __LINE__);									\
	if (severity == BBE::BBUtility::LogAssert)																						\
		assert(0 && message);																									\
} while (0)

#define BB_LogF(handle, severity, message, ...)																					\
do{																																\
	BBE::BBUtility::Logger::GetInstance()->LogF(handle, severity, message, __FILE__, __LINE__, ##__VA_ARGS__);					\
	if (severity == BBE::BBUtility::LogAssert)																						\
		assert(0 && message);																									\
} while (0)

#define BB_Assert(condition, message, ...)																						\
do{																																\
	if(!condition){																												\
		BBE::BBUtility::Logger::GetInstance()->LogF(0, BBUtility::LogAssert, message, __FILE__, __LINE__, ##__VA_ARGS__);		\
		assert(0 && message);																									\
	}																															\
} while (0)


namespace BBE {
	namespace BBUtility {

		using HANDLE = void*;
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
			~Logger();
			static Logger* GetInstance();

			char LogFlagNames[5][16]
			{
				"Info",
				"Warning Low",
				"Warning Medium",
				"Warning High",
				"Assert"
			};

			void SetupLogger(const std::string& loggerName, const WarningTypeFlags& loggerMinimumFlag = LogFlag::LogInfo, const std::string& loggerFileLocation = "");
			void RegisterChannel(const char* a_Name, ChannelHandle& a_Handle);

			void PrintToFile(const std::string& a_Message);

			void Log(const ChannelHandle a_Handle, const LogFlag& flag, const std::string& logMessage, const char* a_File = "", const int& a_Line = 0);
			void LogF(const ChannelHandle a_Handle, const LogFlag& flag, const std::string& logMessage, const char* a_File = "", const int& a_Line = 0, ...);

		private:
			bool ValidFilter(const ChannelHandle& a_Handle, const LogFlag& a_Flag);
			std::string FormatLogMessage(const LogFlag& a_LogFlag, const ChannelHandle& a_ChannelHandle, const std::string& a_LogMessage, const char* a_File, const int& a_Line);

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
			std::string m_DirectoryPath;
			std::string m_FilePath;

			std::thread m_LoggerThread;

			HANDLE m_FileHandle;
		};
	}
}