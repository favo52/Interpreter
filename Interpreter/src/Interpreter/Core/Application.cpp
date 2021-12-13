#include "pch.h"
#include "Application.h"

namespace Interpreter
{
	Application::Application() :
		m_IsRunning{ true }
	{
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			std::string msg{};
			std::cout << ">";
			std::getline(std::cin, msg);

			if (msg == "q" || msg == "Q")
				return;

			LOG_TRACE(msg);
			LOG_INFO(msg);
			LOG_WARN(msg);
			LOG_ERROR(msg);
			LOG_CRITICAL(msg);
			std::cout << "\n";
		}
	}
}
