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
			LOG_TRACE("Please enter the filepath to load.");
			std::cout << "\n>";
			std::getline(std::cin, filepath);

			if (filepath == "q" || filepath == "Q")
				return;

			m_Interpreter.LoadFile(filepath);
			std::cout << '\n';
			m_Interpreter.ReadFile();
			m_Interpreter.CloseFile();

			Quit();
		}
	}
}
