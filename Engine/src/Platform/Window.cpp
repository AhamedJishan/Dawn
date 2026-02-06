#include "Window.h"
#include "Utils/Log.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Input.h"

namespace Dawn
{
	const char* TITLE = "Dawn";

	void GLFWErrorCallback(int error, const char* description)
	{
		LOG_ERROR("[GLFW] %s", description);
	}

	Window::Window()
		:mScreenWidth(1280)
		,mScreenHeight(720)
	{
	}

	Window::~Window()
	{
		if (mWindow)
			glfwDestroyWindow(mWindow);

		glfwTerminate();
	}
	
	bool Window::Init(int screenWidth, int screenHeight)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		// GLFW Init
		glfwSetErrorCallback(GLFWErrorCallback);
		if (!glfwInit())
		{
			LOG_ERROR("Failed to Initialise glfw");
			return false;
		}

		// Opengl 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Window Create
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, TITLE, NULL, NULL);
		if (!mWindow)
		{
			LOG_ERROR("Failed to create window");
			return false;
		}
		glfwMakeContextCurrent(mWindow);
		// Load Opengl
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to Initialize Opengl");
			return false;
		}

		glfwSetWindowUserPointer(mWindow, this);
		glfwSetFramebufferSizeCallback(mWindow, OnFrameBufferResize);

		Input::Init(mWindow);

		return true;
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(mWindow);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(mWindow);
	}

	void Window::GetFrameBufferSize(int& width, int& height) const
	{
		glfwGetFramebufferSize(mWindow, &width, &height);
		GLFW_KEY_0;
	}

	double Window::GetTime()
	{
		return glfwGetTime();
	}

	void Window::OnFrameBufferResize(GLFWwindow* glfwWindow, int width, int height)
	{
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

		if (window->mFrameBufferSizeCallback)
			window->mFrameBufferSizeCallback(width, height);
	}
}