#include "Gun.h"
#include "Utils/Log.h"

#include <glm/gtc/constants.hpp>
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Scene.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/Camera.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "Projectile.h"

namespace Dawn
{
	Gun::Gun(Scene* scene)
		:Actor(scene)
	{
		MeshRenderer::CreateFromModel(this, "Assets/Models/gun/gun.obj");
	}

	void Gun::Update(float deltaTime)
	{
		mTimeSinceLastFire += deltaTime;
		if (mTimeSinceLastFire >= mFireCooldown && Input::GetMouseButtonDown(MouseButton::Left))
		{
			Fire();
			mTimeSinceLastFire = 0.0f;

			mRecoilTimer = mRecoilDuration;
		}

		// No need to store base position.
		// PlayerActor resets the gun transform every frame.
		// Recoil applies a temporary offset on top of that.
		if (mRecoilTimer > 0.0f)
		{
			float t = 1 - mRecoilTimer / mRecoilDuration;
			float recoilOffset = mRecoilAmount * glm::sin(t * glm::pi<float>());
			SetPosition(GetPosition() - GetForward() * recoilOffset);
			mRecoilTimer -= deltaTime;
		}
	}

	void Gun::Fire()
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

		glm::vec3 targetLocation;

		if (mScene->RayCast(ray, 100.0f, hitinfo))
			targetLocation = hitinfo.position;
		else
			targetLocation = ray.origin + ray.direction * 100.0f;

		glm::vec3 projectileDirection = glm::normalize(targetLocation - GetPosition());
		glm::quat projectileRotation = glm::quatLookAt(projectileDirection, glm::vec3(0, 1, 0));

		Projectile* projectile = new Projectile(mScene);
		projectile->SetPosition(GetPosition());
		projectile->SetRotation(projectileRotation);
	}

}