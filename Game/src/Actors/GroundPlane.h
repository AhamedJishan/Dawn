#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"
#include "Rendering/Materials/PhongMaterial.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class GroundPlane : public Actor
	{
	public:
		GroundPlane(Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/greybox/greybox.obj");

			PhongMaterial* mat = dynamic_cast<PhongMaterial*>(GetComponent<MeshRenderer>()->GetMaterial());
			if (mat)
				mat->SetDiffuseColor(glm::vec3(0.5f));
		}
	};
}