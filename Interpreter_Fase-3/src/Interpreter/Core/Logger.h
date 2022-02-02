#pragma once

#include "spdlog/spdlog.h"

namespace Interpreter
{
	class Logger
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

// Log macros
#define LOG_TRACE(...)	  ::Interpreter::Logger::GetLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...)	  ::Interpreter::Logger::GetLogger()->info(__VA_ARGS__);
#define LOG_WARN(...)	  ::Interpreter::Logger::GetLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...)	  ::Interpreter::Logger::GetLogger()->error(__VA_ARGS__);
#define LOG_CRITICAL(...) ::Interpreter::Logger::GetLogger()->critical(__VA_ARGS__);
