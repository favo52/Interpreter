// Interpreter's entry-point

#include "pch.h"
#include "Core/Application.h"

//Xaymarie

int main()
try
{
	Interpreter::Logger::Init();
	LOG_TRACE("Welcome to the LAO interpreter!");

	Interpreter::Application* App = new Interpreter::Application();
	App->Run();
	delete App;

	// This line wasn't here before

	return 0;
}
catch (std::exception& e)
{
	std::cerr << "EXCEPTION: " << e.what() << std::endl;
	return 1;
}
