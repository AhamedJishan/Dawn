#pragma once

namespace Dawn
{
	// Forward declarations
	class Window;
	class Renderer;
	class InputSystem;
	class ImGuiSystem;
	class AssetManager;
	class LayerStack;
	class Layer;

	class Application
	{
	public:
		Application(struct WindowConfig windowConfig);
		virtual ~Application();

		void Run();

		static Application* Get() { return sInstance; }
		Window* GetWindow() const { return mWindow; }
		InputSystem* GetInputSystem() const { return mInputSystem; }
		AssetManager* GetAssetManager() const { return mAssetManager; }
		Renderer* GetRenderer() const { return mRenderer; }

		// Pushes a layer and transfers ownership to the engine.
		void PushLayer(Layer* layer);
		// Pops and destroys the topmost layer
		void PopLayer();

	private:
		void Update();
		void GenerateOutput();

	private:
		static Application* sInstance;

		Window* mWindow;
		Renderer* mRenderer;
		InputSystem* mInputSystem;
		ImGuiSystem* mImGuiSystem;
		AssetManager* mAssetManager;
		LayerStack* mLayerStack;

		bool mIsRunning;
		double mTime;
	};

	// To be defined by the Client;
	Application* CreateApplication();
}