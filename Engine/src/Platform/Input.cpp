#include "Input.h"

#include <cstring>
#include <GLFW/glfw3.h>

namespace Dawn::Input
{
	constexpr int NUM_KEYS = static_cast<int>(Key::Count);

	static bool sPreviousKeyboardState[NUM_KEYS] = { 0 };
	static bool sCurrentKeyboardState[NUM_KEYS] = { 0 };

	static GLFWwindow* sWindow = nullptr;

	void Init(GLFWwindow* window)
	{
		sWindow = window;
	}

	void Update()
	{
		if (!sWindow)
			return;

		memcpy(sPreviousKeyboardState, sCurrentKeyboardState, sizeof(sCurrentKeyboardState));

		for (int i = static_cast<int>(Key::First); i < static_cast<int>(Key::Count); i++)
		{
			sCurrentKeyboardState[i] = glfwGetKey(sWindow, i) == GLFW_PRESS;
		}
	}

	bool GetKey(Key key)
	{
		return sCurrentKeyboardState[static_cast<int>(key)];
	}

	bool GetKeyDown(Key key)
	{
		return !sPreviousKeyboardState[static_cast<int>(key)] && sCurrentKeyboardState[static_cast<int>(key)];
	}
	
	bool GetKeyUp(Key key)
	{
		return sPreviousKeyboardState[static_cast<int>(key)] && !sCurrentKeyboardState[static_cast<int>(key)];
	}
}