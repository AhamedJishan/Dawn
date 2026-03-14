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
		glm::vec3 GetGunPosition();

	private:
		GameScene* mGameScene = nullptr;
		Damageable* mDamageable = nullptr;

		float mSpeed = 3.0f;

		FPSCameraActor* mCamera = nullptr;
		glm::vec3 mCameraOffset = glm::vec3(0, 2, 0);

		glm::vec3 mGunOffset = glm::vec3(0.17f, -.1f, 0.18f);
	};
}