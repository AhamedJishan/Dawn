#pragma once

namespace Dawn
{
	class Application
	{
	public:
		Application(struct WindowConfig windowConfig);
		virtual ~Application();

		void Run();

		static Application* Get() { return sInstance; }
		class Window* GetWindow() const { return mWindow; }
		class InputSystem* GetInputSystem() const { return mInputSystem; }
		class AssetManager* GetAssetManager() const { return mAssetManager; }

		// Pushes a layer and transfers ownership to the engine.
		void PushLayer(class Layer* layer);
		// Pops and destroys the topmost layer
		void PopLayer();

	private:
		void Update();
		void GenerateOutput();

	private:
		static Application* sInstance;

		class Window* mWindow;
		class Renderer* mRenderer;
		class InputSystem* mInputSystem;
		class AssetManager* mAssetManager;
		class LayerStack* mLayerStack;

		bool mIsRunning;
		double mTime;
	};

	// To be defined by the Client;
	Application* CreateApplication();
}