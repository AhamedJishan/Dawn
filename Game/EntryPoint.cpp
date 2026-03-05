#pragma once

#include <iostream>
#include "Utils/Log.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "DawnGame.h"

int main(int argc, char** argv)
{
	Dawn::WindowConfig config;
	config.Title = "Dawn";
	config.IconPath = "Assets/Icons/dawn_icon.png";
	config.Width = 1280;
	config.Height = 720;

	Dawn::DawnGame dawn;

	Dawn::Application app(config);
	app.LoadGame(&dawn);
	app.Run();

	// TEMPORARY, to be removed later
	LOG_INFO("Press any key to exit...");
	std::cin.get();

	return 0;
}