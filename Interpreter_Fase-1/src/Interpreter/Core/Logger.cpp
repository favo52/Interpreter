#include "pch.h"
#include "Logger.h"

namespace Interpreter
{
	std::vector<LogEntry> Logger::s_Messages;

	void Logger::Trace(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.Type = LogType::LOG_INFO;
		logEntry.Message = "INTERPRETER: " + message;
		std::cout << logEntry.Message << std::endl;
		s_Messages.push_back(logEntry);
	}

	void Logger::Info(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.Type = LogType::LOG_INFO;
		logEntry.Message = "INTERPRETER: " + message;
		std::cout << "\x1B[32m" << logEntry.Message << "\033[0m" << std::endl;
		s_Messages.push_back(logEntry);
	}

	void Logger::Warn(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.Type = LogType::LOG_WARNING;
		logEntry.Message = "INTERPRETER: " + message;
		std::cout << "\x1B[93m" << logEntry.Message << "\033[0m" << std::endl;
		s_Messages.push_back(logEntry);
	}

	void Logger::Error(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.Type = LogType::LOG_ERROR;
		logEntry.Message = "INTERPRETER: " + message;
		std::cerr << "\x1B[91m" << logEntry.Message << "\033[0m" << std::endl;
		s_Messages.push_back(logEntry);
	}
}
