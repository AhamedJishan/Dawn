#pragma once

#include "Utils/Log.h"
#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Core/Components/MeshRenderer.h"

namespace Dawn
{
	class ExampleActor : public Actor
	{
	public:
		ExampleActor(Scene* scene)
			:Actor(scene)
		{
			LOG_WARN("ExampleActor created...");

			MeshRenderer::CreateFromModel(this, "Assets/Models/cyborg/cyborg.obj");
		}

		~ExampleActor()
		{
			LOG_WARN("ExampleActor destroyed!");
		}
	};
}