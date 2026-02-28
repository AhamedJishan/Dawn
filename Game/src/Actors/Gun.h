#pragma once

#include "Utils/Log.h"
#include "Core/Actor.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/Camera.h"
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
				Shoot();
		}

		void Shoot()
		{
			Camera* camera = mScene->GetActiveCamera();
			if (!camera)
				return;

			float screenX = static_cast<float>(Application::Get()->GetWindow()->GetWidth());
			float screenY = static_cast<float>(Application::Get()->GetWindow()->GetHeight());

			glm::vec3 origin = camera->UnProject(screenX / 2.0f, screenY / 2.0f, 0.0f);
			glm::vec3 end = camera->UnProject(screenX / 2.0f, screenY / 2.0f, 1.0f);

			RaycastHit hitinfo;
			Physics::Ray ray;
			ray.origin = origin;
			ray.direction = glm::normalize(end - origin);

			if (mScene->RayCast(ray, 100.0f, hitinfo))
				LOG_WARN("BANG!!!");
		}
	};
}