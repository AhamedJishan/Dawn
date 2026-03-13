#include "Gun.h"
#include "Utils/Log.h"

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Scene.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/Camera.h"
#include "Core/Components/Audio.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "PlayerActor.h"
#include "Projectile.h"

namespace Dawn
{
	Gun::Gun(Scene* scene, PlayerActor* player)
		:Actor(scene)
	{
		MeshRenderer::CreateFromModel(this, "Assets/Models/gun/gun.obj");
		mAudioComponent = new Audio(this);
		mPlayer = player;
	}

	void Gun::Update(float deltaTime)
	{
		SyncTransform();

		mTimeSinceLastFire += deltaTime;
		if (mTimeSinceLastFire >= mFireCooldown && Input::GetMouseButtonDown(MouseButton::Left))
		{
			Fire();
			mTimeSinceLastFire = 0.0f;

			mRecoilOffset -= GetForward() * mRecoilKickAmount;
			mRecoilPitch += mRecoilPitchKick;
		}

		mRecoilOffset = glm::mix(mRecoilOffset, glm::vec3(0.0f), deltaTime * mRecoilRecoverySpeed);
		mRecoilPitch = glm::mix(mRecoilPitch, 0.0f, deltaTime * mRecoilRecoverySpeed);

		// Apply recoil
		SetPosition(GetPosition() + mRecoilOffset);
		Rotate(mRecoilPitch, glm::vec3(1, 0, 0));
	}

	void Gun::Fire()
	{
		mAudioComponent->PlayEvent("event:/gunshot");

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

		Projectile* projectile = new Projectile(mScene, mDamage);
		projectile->SetPosition(GetPosition());
		projectile->SetRotation(projectileRotation);
	}

	void Gun::SyncTransform()
	{
		SetPosition(mPlayer->GetGunPosition());
		SetRotation(mScene->GetActiveCamera()->GetOwner()->GetRotation());
	}

}