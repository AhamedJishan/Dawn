#pragma once

#include "Core/Actor.h"

namespace Dawn
{
	// Forward declarations
	class Gun;
	class Arena;
	class GameScene;
	class FPSCameraActor;
	class Damageable;
	class KillStreak;

	class Player : public Actor
	{
	public:
		Player(GameScene* scene, FPSCameraActor* fpsCameraActor, Arena* arena);

		void Update(float deltaTime) override;

		void TakeDamage(float dmg);
		glm::vec3 GetGunPosition();

		Arena* GetArena() const { return mArena; }

	private:
		void DashStart();
		void DashUpdate(float deltaTime);
		void DashEnd();

	private:
		GameScene* mGameScene = nullptr;
		Damageable* mDamageable = nullptr;
		KillStreak* mKillStreak = nullptr;
		Arena* mArena = nullptr;

		FPSCameraActor* mCamera = nullptr;
		glm::vec3 mCameraOffset = glm::vec3(0, 2, 0);

		glm::vec3 mGunOffset = glm::vec3(0.1f, -.125f, 0.18f);

		unsigned int mLastKillStreak = 0;

		float mSpeed = 5.0f;

		// Dash
		bool mIsDashing = false;
		const float mDashCooldownDuration = 5.0f;
		const float mDashDuration = 0.3f;
		const float mDashDistance = 10.0f;
		const float mDashFOVBoost = 8.0f;
		float mCameraBaseFOV;

		float mDashCooldownTimer = 0.0f;
		float mDashTimer = 0.0f;
		glm::vec3 mDashStartPos = glm::vec3(0);
		glm::vec3 mDashEndPos = glm::vec3(0);
	};
}