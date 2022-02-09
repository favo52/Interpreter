#pragma once

#include "Interpreter/Interpreter/Interpreter.h"

int main(int argc, char** argv);

namespace Interpreter
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Quit() { m_IsRunning = false; }

		void Error(const std::string& s) { throw std::runtime_error(s); }
		void ClearInput() { std::cin.clear(); }

		static Application& Get() { return *s_Instance; }

	private:
		void Run();

	private:
		bool m_IsRunning;
		Interpreter m_Interpreter;

		static Application* s_Instance; // pointer to this
		friend int ::main(int argc, char** argv);
	};

	// Disguises runtime_error
	#define ERROR(...) Application::Get().Error(__VA_ARGS__)
}
