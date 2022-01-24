#pragma once

#include <string>
#include <vector>

namespace Interpreter
{
	enum class LogType
	{
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR
	};

	struct LogEntry
	{
		LogType Type;
		std::string Message;
	};

	class Logger
	{
	public:
		static void Trace(const std::string& message);
		static void Info(const std::string& message);
		static void Warn(const std::string& message);
		static void Error(const std::string& message);

	private:
		static std::vector<LogEntry> s_Messages;
	};
}

// Log macros
#define LOG_TRACE(...)	::Interpreter::Logger::Trace(__VA_ARGS__);
#define LOG_INFO(...)	::Interpreter::Logger::Info(__VA_ARGS__);
#define LOG_WARN(...)	::Interpreter::Logger::Warn(__VA_ARGS__);
#define LOG_ERROR(...)	::Interpreter::Logger::Error(__VA_ARGS__);
