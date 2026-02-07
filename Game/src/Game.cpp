#pragma once

#include "Utils/Log.h"
#include "EntryPoint.h"
#include "Core/Application.h"

namespace Dawn
{
	class Game : public Application
	{
	public:
		Game(int screenWidth, int screenHeight)
			:Application(screenWidth, screenHeight)
		{
			// TODO: Push layers
		}

		~Game()
		{
		}
	};


	// ==================================
	//			Game Entry Point
	// ==================================
	Application* CreateApplication()
	{
		LOG_INFO("Creating game");
		return new Game(1280, 720);
	}
}