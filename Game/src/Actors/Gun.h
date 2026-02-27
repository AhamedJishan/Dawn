#pragma once

#include "Utils/Log.h"
#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"
#include "Physics/Physics.h"

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

		void Update(float deltaTime) override
		{
			if (Input::GetMouseButtonDown(MouseButton::Left))
			{
				RaycastHit hitinfo;
				Physics::Ray ray;
				ray.origin = GetPosition();
				ray.direction = GetForward();

				if (mScene->RayCast(ray, 100.0f, hitinfo))
					LOG_WARN("BANG!!!");
			}
		}
	};
}