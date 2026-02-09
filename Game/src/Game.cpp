#pragma once

#include "Utils/Log.h"
#include "EntryPoint.h"
#include "Core/Application.h"
#include "Core/Window.h"

#include "ExampleLayer.h"

namespace Dawn
{
	class Game : public Application
	{
	public:
		Game(WindowConfig windowConfig)
			:Application(windowConfig)
		{
			// Push layers
			PushLayer(new ExampleLayer());
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
		WindowConfig window;
		window.Width = 1280;
		window.Height = 720;
		window.Title = "Dawn";
		window.IconPath = "../Assets/icons/dawn_icon.png";

		LOG_INFO("Creating game");
		return new Game(window);
	}
}