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
		inline void Quit() { m_IsRunning = false; }

	private:
		bool m_IsRunning;

		Interpreter m_Interpreter;
	};
}
