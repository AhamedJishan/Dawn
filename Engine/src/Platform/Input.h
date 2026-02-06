#pragma once

#include "KeyCodes.h"

// Forward declaration
struct GLFWwindow;

namespace Dawn
{
	namespace Input
	{
		// Initialized by Window
		void Init(GLFWwindow* window);
		// Updated by Application
		void Update();

		bool GetKey(Key key);
		bool GetKeyDown(Key key);
		bool GetKeyUp(Key key);
	}
}