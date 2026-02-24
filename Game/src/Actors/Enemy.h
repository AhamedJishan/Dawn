#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"
#include "Rendering/Materials/PhongMaterial.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class Enemy : public Actor
	{
	public:
		Enemy(Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/enemy/enemy.obj");
		}
	};
}