#include "PlayerActor.h"
#include "Utils/Log.h"

#include "Gun.h"
#include "Core/Scene.h"
#include "FPSCameraActor.h"
#include "Components/Damageable.h"

namespace Dawn
{
	PlayerActor::PlayerActor(Scene* scene, FPSCameraActor* fpsCameraActor)
		:Actor(scene)
	{
		if (!fpsCameraActor)
		{
			LOG_WARN("Player Actor needs valid 'FPSCameraActor'");
			return;
		}

		mCamera = fpsCameraActor;

		mGun = new Gun(mScene);
		mDamageable = new Damageable(this, 100.0f);
	}

	void PlayerActor::Update(float deltaTime)
	{
		// sync camera pos
		mCamera->SetPosition(GetPosition() + mCameraOffset);

		float yaw = mCamera->GetYaw();
		SetRotation(glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)));

		// WASD movement
		float speed = mSpeed;
		if (Input::GetKey(Key::LeftShift)) speed = 2 * mSpeed;
		if (Input::GetKey(Key::W)) mPosition += speed * GetForward() * deltaTime;
		if (Input::GetKey(Key::S)) mPosition -= speed * GetForward() * deltaTime;
		if (Input::GetKey(Key::A)) mPosition -= speed * GetRight() * deltaTime;
		if (Input::GetKey(Key::D)) mPosition += speed * GetRight() * deltaTime;

		// sync Gun pos
		glm::vec3 gunPos = mCamera->GetPosition();
		gunPos += mCamera->GetUp() * mGunOffset.y;
		gunPos += mCamera->GetRight() * mGunOffset.x;
		gunPos += mCamera->GetForward() * mGunOffset.z;
		mGun->SetPosition(gunPos);
		// sync gun rotation
		mGun->SetRotation(mCamera->GetRotation());
	}

	void PlayerActor::TakeDamage(float dmg)
	{
		mDamageable->TakeDamage(dmg);
		if (mDamageable->IsDead())
		{
			// TODO: transition to gameover
			LOG_WARN("GAME OVER!!");
		}
	}
}