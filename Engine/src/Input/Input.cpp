#include "Input.h"

#include "Core/Application.h"
#include "InputSystem.h"

namespace Dawn
{
	bool Input::GetKey(Key key)
	{
		return Application::Get()->GetInputSystem()->GetKeyBoard().GetKey(key);
	}

	bool Input::GetKeyDown(Key key)
	{
		return Application::Get()->GetInputSystem()->GetKeyBoard().GetKeyDown(key);
	}

	bool Input::GetKeyUp(Key key)
	{
		return Application::Get()->GetInputSystem()->GetKeyBoard().GetKeyUp(key);
	}
}