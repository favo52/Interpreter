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
			LOG_TRACE("Please enter the filepath to load the file. (Exit: \"--q\")");
			std::cout << ">";
			std::string filepath{};
			std::getline(std::cin, filepath);

			if (filepath.empty()) continue;
			if (filepath == QUIT) { LOG_INFO("Quitting..."); return; };
			if (!m_Interpreter.LoadFile(filepath)) continue;

			std::cout << '\n';
			LOG_INFO("Program start");
			m_Interpreter.ReadFile();
			m_Interpreter.CloseFile();

			std::cout << '\n';
			LOG_WARN("Restarting...");
			m_Interpreter.Reset();
		}
		catch (std::exception& e)
		{
			LOG_ERROR("ERROR: {0}", e.what());
			ClearInput();
		}
	}
}
