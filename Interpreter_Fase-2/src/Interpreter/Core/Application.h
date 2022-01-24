#pragma once

#include "Interpreter/Interpreter.h"

namespace Interpreter
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void Quit() { m_IsRunning = false; }

		void Error(const std::string& s) { throw std::runtime_error(s); }
		void ClearInput() { std::cin.clear(); }

		static Application& Get() { return *s_Instance; }

	private:
		bool m_IsRunning{ true };
		Interpreter m_Interpreter;

		static Application* s_Instance; // pointer to this
	};

	// Disguises runtime_error
	#define ERROR(...) Application::Get().Error(__VA_ARGS__)
}
