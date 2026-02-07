#pragma once

#include "Keyboard.h"

namespace Dawn
{
	class InputSystem
	{
	public:
		// Not needed right now
		// void Init();

		void Update();

		const Keyboard& GetKeyBoard() { return mKeyboard; }

	private:
		Keyboard mKeyboard;
	};
}