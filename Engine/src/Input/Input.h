#pragma once

#include "KeyCodes.h"

namespace Dawn
{
	// Facade for InputSystem
	class Input
	{
	public:
		static bool GetKey(Key key);
		static bool GetKeyDown(Key key);
		static bool GetKeyUp(Key key);
	};
}