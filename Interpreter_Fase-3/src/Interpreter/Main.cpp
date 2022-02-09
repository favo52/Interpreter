// Interpreter's entry-point

#include "pch.h"
#include "Core/Application.h"

int main(int argc, char** argv)
{
	try
	{
		Interpreter::Logger::Init();
		LOG_TRACE("Welcome to the LAO Interpreter Phase 3!");

		Interpreter::Application* App = new Interpreter::Application();
		App->Run();
		delete App;
	}
	catch (std::exception& e)
	{
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
