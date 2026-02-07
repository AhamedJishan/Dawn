#include "Application.h"
#include "Utils/Log.h"

#include "Window.h"
#include "Rendering/Renderer.h"
#include "Input/InputSystem.h"
#include "Input/Input.h"

namespace Dawn
{
	Application* Application::sInstance = nullptr;

	// TODO: Application takes in init parameters like screen size
	Application::Application(int screenWidth, int screenHeight)
		:mIsRunning(true)
		,mTime(0.0)
	{
		if (sInstance)
		{
			LOG_ERROR("Application already exists!");
			mIsRunning = false;
			return;
		}

		sInstance = this;

		mWindow = new Window();
		if (!mWindow->Init(screenWidth, screenHeight))
		{
			mIsRunning = false;
			return;
		}

		mRenderer = new Renderer();
		if (!mRenderer->Init())
		{
			mIsRunning = false;
			return;
		}

		mInputSystem = new InputSystem();
	}

	Application::~Application()
	{
		if (mWindow)
			delete mWindow;

		if (mRenderer)
			delete mRenderer;
	}

	void Application::Run()
	{
		// TODO: Actual lifecyle
		while (mIsRunning)
		{
			mWindow->PollEvents();
			mInputSystem->Update();

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
		// Prevent large deltaTime jumps
		deltaTime = deltaTime > 0.05 ? 0.05 : deltaTime;

		// Input system test
		if (Input::GetKeyDown(Key::Space))
			LOG_INFO("Application Running: %f", 1/deltaTime);
	}

	void Application::GenerateOutput()
	{
		mRenderer->Draw();
	}
}