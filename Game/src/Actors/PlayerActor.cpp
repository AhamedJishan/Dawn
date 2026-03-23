#include "PlayerActor.h"
#include "Utils/Log.h"

#include "Scenes/GameScene.h"
#include "FPSCameraActor.h"
#include "Components/Damageable.h"
#include "Components/KillStreak.h"
#include "Core/Components/SphereCollider.h"
#include "Arena.h"

namespace Dawn
{
	PlayerActor::PlayerActor(GameScene* scene, FPSCameraActor* fpsCameraActor, Arena* arena)
		:Actor(scene)
	{
		if (!fpsCameraActor)
		{
			LOG_WARN("Player Actor needs valid 'FPSCameraActor'");
			return;
		}

		mGameScene = scene;
		mCamera = fpsCameraActor;
		mArena = arena;
		mDamageable = new Damageable(this, 100.0f);
		mKillStreak = new KillStreak(this);

		SphereCollider* collider = new SphereCollider(this);
		collider->SetIsDynamic(true);
		collider->SetIsTrigger(false);

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
		if (Input::GetKey(Key::W)) mPosition += mSpeed * GetForward() * deltaTime;
		if (Input::GetKey(Key::S)) mPosition -= mSpeed * GetForward() * deltaTime;
		if (Input::GetKey(Key::A)) mPosition -= mSpeed * GetRight() * deltaTime;
		if (Input::GetKey(Key::D)) mPosition += mSpeed * GetRight() * deltaTime;

		// Resolve Arena Bound Collision
		if (mArena->IsOutOfBounds(GetPosition()))
		{
			glm::vec3 pos = GetPosition();
			glm::vec2 bounds = mArena->GetBounds();

			if (pos.x > bounds.x)
				pos.x = bounds.x;
			if (pos.x < -bounds.x)
				pos.x = -bounds.x;
			if (pos.z > bounds.y)
				pos.z = bounds.y;
			if (pos.z < -bounds.y)
				pos.z = -bounds.y;

			SetPosition(pos);
		}
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