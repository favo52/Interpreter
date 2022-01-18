#include "pch.h"
#include "Application.h"

namespace Interpreter
{
	Application* Application::s_Instance{ nullptr };

	Application::Application() :
		m_IsRunning{ true },
		m_Interpreter{}
	{
		assert(!s_Instance);
		s_Instance = this;
	}

	void Application::RunPhase1()
	{
		std::cout << '\n';
		LOG_INFO("Program start");
		while (m_IsRunning)
		{
			std::cout << '\n';
			LOG_TRACE("Please enter the word to interpret. (Exit: \"--q\")");
			std::cout << ">";
			std::string line;
			std::getline(std::cin, line);

			if (line.empty()) continue;
			if (line == "--q") { LOG_INFO("Program end"); return; };

			// Deal with strings
			if (line.front() == '\"')
			{
				m_Interpreter.ReadWord(line);
			}
			else // Deal with everything else
			{
				bool printNewline{ false };
				std::istringstream iss{ line };
				for (std::string word; iss >> word;)
				{
					if (printNewline) std::cout << "\n";
					if (!iss.eof()) printNewline = true;
					m_Interpreter.ReadWord(word);
				}
			}
#if OLD
			if (std::cin >> word)
				m_Interpreter.ReadWord(word);
#endif
		}
	}

	void Application::RunPhase2()
	{
		while (m_IsRunning)
		{
			std::cout << '\n';
			LOG_TRACE("Please enter a line to interpret.");
			std::cout << ">";
			std::string line;
			std::getline(std::cin, line);

			if (line.empty()) continue;

			m_Interpreter.ReadLine();
		}
	}

	void Application::RunPhase3()
	{
		while (m_IsRunning)
		{
			std::cout << '\n';
			LOG_TRACE("Please enter the filepath to load the file.");
			std::cout << ">";
			std::string filepath{};
			std::getline(std::cin, filepath);

			if (filepath.empty()) continue;
			if (filepath == "q" || filepath == "Q") { LOG_INFO("Quitting..."); return; }
			if (!m_Interpreter.LoadFile(filepath)) continue;

			std::cout << '\n';
			LOG_INFO("Program start");
			m_Interpreter.ReadFile();
			m_Interpreter.CloseFile();

			std::cout << '\n';
			LOG_WARN("Restarting...");
			m_Interpreter.Reset();

			//Quit();
		}
	}
}
