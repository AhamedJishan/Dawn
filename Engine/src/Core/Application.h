#pragma once

#include <vector>

namespace Dawn
{
	class Application
	{
	public:
		Application(struct WindowConfig windowConfig);
		virtual ~Application();

		void Run();

		static Application* Get() { return sInstance; }
		class Window* GetWindow() { return mWindow; }
		class InputSystem* GetInputSystem() { return mInputSystem; }

	private:
		void Update();
		void GenerateOutput();

	private:
		static Application* sInstance;

		class Window* mWindow;
		class Renderer* mRenderer;
		class InputSystem* mInputSystem;

		bool mIsRunning;
		double mTime;
	};

	// To be defined by the Client;
	Application* CreateApplication();
}