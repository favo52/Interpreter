#include "pch.h"
#include "Application.h"

namespace Interpreter
{
	constexpr const char* QUIT{ "--q" };

	Application* Application::s_Instance{ nullptr };

	Application::Application() :
		m_IsRunning{ true }
	{
		assert(!s_Instance);
		s_Instance = this;
	}

	void Application::Run()
	{
		// Main program loop
		while (m_IsRunning) try
		{
			// Grab entire input
			std::cout << '\n';
			LOG_TRACE("Please enter the filepath. (Exit: \"--q\")");
			std::cout << ">";
			std::string filepath{};
			std::getline(std::cin, filepath);

			if (filepath.empty()) continue;
			if (filepath == QUIT) { LOG_INFO("Quitting..."); return; };
			if (!m_Interpreter.OpenFile(filepath)) continue;

			std::cout << '\n';
			LOG_INFO("Program start");
			m_Interpreter.ReadFile();
			m_Interpreter.CloseFile();

			std::cout << '\n';
			LOG_WARN("Restarting...");
			m_Interpreter.Reset();
			ClearInput();
		}
		catch (std::exception& e)
		{
			std::cout << '\n';
			LOG_ERROR("Line {0}: {1}", m_Interpreter.GetLineNumber(), m_Interpreter.GetLine());
			LOG_ERROR("ERROR: {0}", e.what());
			m_Interpreter.CloseFile();
			m_Interpreter.Reset();
			ClearInput();
		}
	}
}
