#include "Application.h"
#include "Utils/Log.h"

#include <glm/vec2.hpp>
#include "Window.h"
#include "Rendering/Renderer.h"
#include "Input/InputSystem.h"
#include "Input/Input.h"
#include "AssetManager.h"
#include "LayerStack.h"
#include "Layer.h"

namespace Dawn
{
	Application* Application::sInstance = nullptr;

	// TODO: Application takes in init parameters like screen size
	Application::Application(WindowConfig windowConfig)
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
		if (!mWindow->Init(windowConfig))
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
		mAssetManager = new AssetManager();
		mLayerStack = new LayerStack();
	}

	Application::~Application()
	{
		if (mLayerStack)	delete mLayerStack;
		if (mAssetManager)	delete mAssetManager;
		if (mInputSystem)	delete mInputSystem;
		if (mRenderer)		delete mRenderer;
		if (mWindow)		delete mWindow;

		sInstance = nullptr;
	}

	void Application::Run()
	{
		while (mIsRunning)
		{
			Update();
			GenerateOutput();
		}

	}

	void Application::Update()
	{
		mWindow->PollEvents();

		if (mWindow->ShouldClose())
			mIsRunning = false;

		double currentTime = mWindow->GetTime();
		double deltaTime = currentTime - mTime;
		mTime = currentTime;
		// Prevent large deltaTime jumps
		deltaTime = deltaTime > 0.05 ? 0.05 : deltaTime;

		mInputSystem->Update();
		mLayerStack->Update(deltaTime);

		if (Input::GetKeyDown(Key::Space))
			LOG_INFO("FPS: %f", (1 / deltaTime));
	}

	void Application::GenerateOutput()
	{
		mRenderer->Draw();

		mWindow->SwapBuffers();
	}


	// Layer Functionality
	void Application::PushLayer(Layer* layer)
	{
		mLayerStack->PushLayer(layer);
	}
	void Application::PopLayer()
	{
		mLayerStack->PopLayer();
	}
}