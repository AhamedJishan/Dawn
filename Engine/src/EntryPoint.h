#pragma once

#include "Utils/Log.h"

#include "Core/Application.h"

extern Dawn::Application* Dawn::CreateApplication();

int main(int argc, char** argv)
{
	
	auto app = Dawn::CreateApplication();
	app->Run();
	delete app;

	return 0;
}