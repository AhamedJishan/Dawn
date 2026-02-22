#pragma once

#include "Utils/Log.h"
#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Core/Components/Camera.h"

namespace Dawn
{
	class TestCameraActor : public Actor
	{
	public:
		TestCameraActor(Scene* scene)
			:Actor(scene)
		{
			LOG_WARN("Camera Actor created...");

			new Camera(this);
		}

		~TestCameraActor()
		{
			LOG_WARN("Camera Actor destroyed!");
		}
	};
}