#pragma once

#include <vector>

namespace Dawn
{
	/// <summary>
	/// Owns the main Application lifecycle, and runs the attached layer
	/// </summary>
	class Application
	{
	public:
		Application();
		~Application();

		bool Init(int screenWidth, int screenHeight);

		void Run();
		
		// TODO: Add functionality for Layers

		class Window* GetWindow() { return mWindow; }

	private:

	private:
		class Window* mWindow;
		class Renderer* mRenderer;

		bool mIsRunning;
	};
}