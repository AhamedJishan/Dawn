#pragma once

#include <glm/vec3.hpp>
#include "Utils/Log.h"
#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Core/Components/MeshRenderer.h"
#include "Rendering/Materials/PhongMaterial.h"

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

			PhongMaterial* mat = dynamic_cast<PhongMaterial*>(GetComponent<MeshRenderer>()->GetMaterial());
			if (mat)
			{
				mat->SetSpecularColor(glm::vec3(1.0f));
				mat->SetDiffuseColor(glm::vec3(1));
			}
		}

		~ExampleActor()
		{
			LOG_WARN("ExampleActor destroyed!");
		}
	};
}