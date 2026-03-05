#pragma once

namespace Dawn
{
	// Forward declarations
	class Game;
	class Window;
	class Renderer;
	class InputSystem;
	class ImGuiSystem;
	class AssetManager;

	class Application
	{
	public:
		Application(struct WindowConfig windowConfig);
		virtual ~Application();

		void LoadGame(Game* game);

		void Run();

		static Application* Get() { return sInstance; }
		Window* GetWindow() const { return mWindow; }
		InputSystem* GetInputSystem() const { return mInputSystem; }
		AssetManager* GetAssetManager() const { return mAssetManager; }
		Renderer* GetRenderer() const { return mRenderer; }

	private:
		void Update();
		void GenerateOutput();

	private:
		static Application* sInstance;

		Game* mGame = nullptr;

		Window* mWindow = nullptr;
		Renderer* mRenderer = nullptr;
		InputSystem* mInputSystem = nullptr;
		ImGuiSystem* mImGuiSystem = nullptr;
		AssetManager* mAssetManager = nullptr;

		bool mIsRunning = false;
		double mTime = 0.0;
	};
}