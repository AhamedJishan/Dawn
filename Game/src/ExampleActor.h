#pragma once

#include "Utils/Log.h"
#include "Core/Actor.h"
#include "Core/Scene.h"

namespace Dawn
{
	class ExampleActor : public Actor
	{
	public:
		ExampleActor(Scene* scene)
			:Actor(scene)
		{
			LOG_WARN("ExampleActor created...");
		}

		~ExampleActor()
		{
			LOG_WARN("ExampleActor destroyed!");
		}
	};
}