#include "pch.h"
#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Interpreter
{
	std::shared_ptr<spdlog::logger> Logger::s_Logger;

	void Logger::Init()
	{
		try
		{
			spdlog::set_pattern("%^%n: %v%$");

			s_Logger = spdlog::stdout_color_mt("Interpreter");
			s_Logger->set_level(spdlog::level::trace);
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cerr << "Log initialization failed: " << ex.what() << std::endl;
		}
	}
}
