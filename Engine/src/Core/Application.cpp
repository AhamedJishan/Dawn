#include "Application.h"
#include "Utils/Log.h"

#include "Platform/Window.h"
#include "Rendering/Renderer.h"
#include "Platform/Input.h"

namespace Dawn
{
	// TODO: Application takes in init parameters like screen size
	Application::Application()
		:mIsRunning(true)
		,mTime(0.0)
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
			return false;

		mRenderer = new Renderer(this);
		if (!mRenderer->Init())
			return false;

		return true;
	}

	void Application::Run()
	{
		// TODO: Actual lifecyle
		while (mIsRunning)
		{
			mWindow->PollEvents();
			Input::Update();

			Update();
			GenerateOutput();

			mWindow->SwapBuffers();
		}

	}

	void Application::Update()
	{
		if (mWindow->ShouldClose())
			mIsRunning = false;

		double currentTime = mWindow->GetTime();
		double deltaTime = currentTime - mTime;
		mTime = currentTime;
		// Makes sure time dependent stuff doesn't go haywire with too big of a deltaTime
		deltaTime = deltaTime > 0.05 ? 0.05 : deltaTime;

		if (Input::GetKeyDown(Key::Space))
			LOG_INFO("Application Running: %f", 1/deltaTime);
	}

	void Application::GenerateOutput()
	{
		mRenderer->Draw();
	}
}