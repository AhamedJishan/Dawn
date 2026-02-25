#pragma once

#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class Gun : public Actor
	{
	public:
		Gun(Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/gun/gun.obj");
		}
	};
}