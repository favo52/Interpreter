// Interpreter's entry-point

#include "pch.h"
#include "Core/Application.h"

int main()
try
{
	LOG_TRACE("Welcome to the LAO interpreter Phase 1!");

	Interpreter::Application* App = new Interpreter::Application();
	App->Run();
	delete App;

	return 0;
}
catch (std::exception& e)
{
	LOG_ERROR("EXCEPTION: " + std::string(e.what()));
	return 1;
}
