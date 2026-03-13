#include "PlayerActor.h"
#include "Utils/Log.h"

#include "Gun.h"
#include "Scenes/GameScene.h"
#include "FPSCameraActor.h"
#include "Components/Damageable.h"

namespace Dawn
{
	PlayerActor::PlayerActor(GameScene* scene, FPSCameraActor* fpsCameraActor)
		:Actor(scene)
	{
		if (!fpsCameraActor)
		{
			LOG_WARN("Player Actor needs valid 'FPSCameraActor'");
			return;
		}

		mGameScene = scene;
		mCamera = fpsCameraActor;
		mDamageable = new Damageable(this, 100.0f);

		mGameScene->SetPlayerHealth(mDamageable->GetHealth());
		mGameScene->SetMaxPlayerHealth(mDamageable->GetMaxHealth());
	}

	void PlayerActor::Update(float deltaTime)
	{
		if (mDamageable->IsDead())
			mGameScene->GameOver();

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
	}

	void PlayerActor::TakeDamage(float dmg)
	{
		mDamageable->TakeDamage(dmg);
		mGameScene->SetPlayerHealth(mDamageable->GetHealth());

		if (mDamageable->IsDead())
		{
			LOG_WARN("PLAYER DIED!");
			mGameScene->GameOver();
		}
	}
	glm::vec3 PlayerActor::GetGunPosition()
	{
		glm::vec3 gunPos = mCamera->GetPosition();

		gunPos += mCamera->GetUp() * mGunOffset.y;
		gunPos += mCamera->GetRight() * mGunOffset.x;
		gunPos += mCamera->GetForward() * mGunOffset.z;

		return gunPos;
	}
}