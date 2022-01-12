#pragma once

#include "Interpreter/Interpreter.h"

namespace Interpreter
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void RunPhase1();
		void RunPhase2();
		void RunPhase3();

		void Quit() { m_IsRunning = false; }

		static Application& Get() { return *s_Instance; }

	private:
		bool m_IsRunning;

		Interpreter m_Interpreter;

	private:
		static Application* s_Instance; // pointer to this
	};
}
