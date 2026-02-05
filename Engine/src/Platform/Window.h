#pragma once

#include <functional>

// Forward declaration
typedef struct GLFWwindow GLFWwindow;

namespace Dawn
{
	class Window
	{
	public:
		using FrameBufferSizeCallbackFn = std::function<void(int width, int height)>;

		Window();
		~Window();

		bool Init(int screenWidth, int screenHeight);

		bool ShouldClose();

		void PollEvents();
		void SwapBuffers();

		void SetFrameBufferSizeCallback(FrameBufferSizeCallbackFn callback) { mFrameBufferSizeCallback = callback; }
		void GetFrameBufferSize(int& width, int& height) const;

	private:
		static void OnFrameBufferResize(GLFWwindow* glfwWindow, int width, int height);

	private:
		int mScreenWidth;
		int mScreenHeight;

		GLFWwindow* mWindow;

		FrameBufferSizeCallbackFn mFrameBufferSizeCallback;
	};
}