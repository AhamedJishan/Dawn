#pragma once

#include <iostream>
#include "Utils/Log.h"

#include "Core/Application.h"

extern Dawn::Application* Dawn::CreateApplication();

int main(int argc, char** argv)
{
	
	auto app = Dawn::CreateApplication();
	app->Run();
	delete app;

	// TEMPORARY, to be removed later
	LOG_INFO("Press any key to exit...");
	std::cin.get();

	return 0;
}