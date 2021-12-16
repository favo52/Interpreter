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
			std::string filepath{};
			std::cout << '\n';
			LOG_TRACE("Please enter the filepath to load.");
			std::cout << ">";
			std::getline(std::cin, filepath);

			if (filepath.empty()) continue;
			if (filepath == "q" || filepath == "Q") { LOG_INFO("Quitting..."); return; }
			if (!m_Interpreter.LoadFile(filepath)) continue;

			std::cout << '\n';
			m_Interpreter.ReadFile();
			m_Interpreter.CloseFile();

			Quit();
		}
	}
}
