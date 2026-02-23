#pragma once

#include <glm/vec3.hpp>
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

		void Update(float deltaTime) override
		{
			Rotate(glm::radians(90.0f) * deltaTime, glm::vec3(0, 1, 0));
		}
	};
}