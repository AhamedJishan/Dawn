#pragma once

#include <vector>

namespace Dawn
{
	class Application
	{
	public:
		Application(int screenWidth, int screenHeight);
		virtual ~Application();

		void Run();

		class Window* GetWindow() { return mWindow; }

	private:
		void Update();
		void GenerateOutput();

	private:
		static Application* sInstance;

		class Window* mWindow;
		class Renderer* mRenderer;

		bool mIsRunning;
		double mTime;
	};

	// To be defined by the Client;
	Application* CreateApplication();
}