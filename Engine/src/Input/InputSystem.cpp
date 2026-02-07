#include "InputSystem.h"

#include <cstring>
#include <GLFW/glfw3.h>
#include "Core/Application.h"
#include "Core/Window.h"

namespace Dawn
{
	//void InputSystem::Init()
	//{
	//}

	void InputSystem::Update()
	{
		std::memcpy(mKeyboard.mPreviousState, mKeyboard.mCurrentState, sizeof(mKeyboard.mCurrentState));


		const int lastKey = static_cast<int>(Key::Last);
		const int firstKey = static_cast<int>(Key::First);

		GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());

		for (int i = firstKey; i <= lastKey; i++)
		{
			int state = glfwGetKey(window, i);
			bool pressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
			mKeyboard.mCurrentState[i] = pressed;
		}
	}
}