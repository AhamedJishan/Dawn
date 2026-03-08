#pragma once

#include "Core/Actor.h"

namespace Dawn
{
	// Forward declarations
	class Gun;
	class GameScene;
	class FPSCameraActor;
	class Damageable;

	class PlayerActor : public Actor
	{
	public:
		PlayerActor(GameScene* scene, FPSCameraActor* fpsCameraActor);

		void Update(float deltaTime) override;

		void TakeDamage(float dmg);

	private:
		GameScene* mGameScene = nullptr;

		float mSpeed = 3.0f;

		FPSCameraActor* mCamera = nullptr;
		glm::vec3 mCameraOffset = glm::vec3(0, 2, 0);

		Gun* mGun = nullptr;
		glm::vec3 mGunOffset = glm::vec3(0.15f, -.075f, 0.225f);

		Damageable* mDamageable = nullptr;
	};
}