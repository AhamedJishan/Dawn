#include "Application.h"
#include "Utils/Log.h"

#include "Game.h"
#include "Window.h"
#include "Rendering/Renderer.h"
#include "ImGui/ImGuiSystem.h"
#include "Input/InputSystem.h"
#include "AssetManager.h"
#include "Scene.h"

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
		mImGuiSystem = new ImGuiSystem();
		mAssetManager = new AssetManager();
	}

	Application::~Application()
	{
		if (mAssetManager)	delete mAssetManager;
		if (mInputSystem)	delete mInputSystem;
		if (mImGuiSystem)	delete mImGuiSystem;
		if (mRenderer)		delete mRenderer;
		if (mWindow)		delete mWindow;

		sInstance = nullptr;
	}

	void Application::LoadGame(Game* game)
	{
		mGame = game;
	}

	void Application::Run()
	{
		// --- GAME START ---
		if (mGame) mGame->Start();

		while (mIsRunning)
		{
			Update();
			GenerateOutput();
		}
	}

	void Application::Update()
	{
		if (mPendingScene)
		{
			if (mScene) delete mScene;

			mScene = mPendingScene;
			mPendingScene = nullptr;
			mScene->Init();
		}

		mWindow->PollEvents();

		if (mWindow->ShouldClose())
			mIsRunning = false;

		double currentTime = mWindow->GetTime();
		double deltaTime = currentTime - mTime;
		mTime = currentTime;
		// Prevent large deltaTime jumps
		deltaTime = deltaTime > 0.05 ? 0.05 : deltaTime;

		mInputSystem->Update();

		if (mScene) mScene->Update(deltaTime);

		// --- GAME UPDATE ---
		if(mGame) mGame->Update(deltaTime);
	}

	void Application::GenerateOutput()
	{
		mRenderer->Draw();

		mImGuiSystem->BeginFrame();
		// --- GAME IMGUIRENDER ---
		if (mGame) mGame->ImGuiRender();
		mImGuiSystem->EndFrame();

		mWindow->SwapBuffers();
	}
}