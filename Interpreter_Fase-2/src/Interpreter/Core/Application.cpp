#include "pch.h"
#include "Application.h"

namespace Interpreter
{
	constexpr const char* QUIT{ "--q" };

	Application* Application::s_Instance{ nullptr };

	Application::Application()
	{
		assert(!s_Instance);
		s_Instance = this;
	}

	void Application::Run()
	{
		std::cout << '\n';
		LOG_INFO("Program start");

		// Main loop
		while (m_IsRunning) try
		{
			// Grab entire input
			std::cout << '\n';
			LOG_TRACE("Please enter the line to interpret. (Exit: \"--q\")");
			std::cout << ">";
			std::string line;
			std::getline(std::cin, line);

			if (line.empty()) continue;
			if (line == QUIT) { LOG_INFO("Program end"); return; };

			// Interpret the word
			m_Interpreter.InterpretLine(line);
			m_Interpreter.Reset();
		}
		catch (std::exception& e)
		{
			LOG_ERROR("ERROR: {0}", e.what());
			ClearInput();
		}
	}
}
