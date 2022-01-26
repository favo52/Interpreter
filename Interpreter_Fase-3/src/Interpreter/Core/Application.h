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

		static Application& Get() { return *s_Instance; }

	private:
		void ClearInput() { std::cin.clear(); }
		void Error(const std::string& s) { throw std::runtime_error(s); }

	private:
		bool m_IsRunning{ true };
		Interpreter m_Interpreter;

		static Application* s_Instance; // pointer to this
	};
}
