#include "Application.h"
#include "Utils/Log.h"

#include "Platform/Window.h"
#include "Rendering/Renderer.h"

namespace Dawn
{
	// TODO: Application takes in init parameters like screen size
	Application::Application()
		:mIsRunning(true)
	{
	}

	Application::~Application()
	{
		if (mWindow)
			delete mWindow;

		if (mRenderer)
			delete mRenderer;
	}

	bool Application::Init(int screenWidth, int screenHeight)
	{
		mWindow = new Window();
		if (!mWindow->Init(screenWidth, screenHeight))
		{
			delete mWindow;
			return false;
		}

		mRenderer = new Renderer(this);
		if (!mRenderer->Init())
		{
			delete mRenderer;
			return false;
		}

		return true;
	}

	void Application::Run()
	{
		// TODO: Actual lifecyle
		while (mIsRunning)
		{
			if (mWindow->ShouldClose())
				mIsRunning = false;

			LOG_INFO("Application Running");

			// TODOD: Move this call to GenerateOutput
			mRenderer->Draw();
		}

	}
}