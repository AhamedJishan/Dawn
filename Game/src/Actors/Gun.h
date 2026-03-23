#pragma once

#include "Core/Actor.h"
#include "glm/vec3.hpp"

namespace Dawn
{
	// Forward declarations
	class PlayerActor;
	class KillStreak;
	class Scene;
	class Audio;

	class Gun : public Actor
	{
	public:
		Gun(Scene* scene, PlayerActor* player);

		void Update(float deltaTime) override;

		void Fire();

	private:
		// Updates 'mSwayMoveOffset' and 'mSwayRotationOffset'
		void UpdateSwayOffsets(float deltaTime);
		glm::vec2 GetSwayMovementOffset();
		glm::vec2 GetSwayRotationOffset();

		void RecoilKickBack();
		void RecoilRecovery(float deltaTime);

		void UpdateBobOffset(float deltaTime);

		glm::vec3 GetMoveInput();

	private:
		PlayerActor* mPlayer = nullptr;
		KillStreak* mPlayerKillStreak = nullptr;
		Audio* mAudioComponent = nullptr;

		float mDamage = 30.0f;
		const float mFireCooldown = 0.1f;
		float mTimeSinceLastFire = 0.0f;
		glm::vec3 mProjectileSpawnOffset = glm::vec3(0.0f, 0.05f, 0.15f);

		// Recoil
		glm::vec3 mRecoilOffset = glm::vec3(0.0f);
		float mRecoilPitch = 0.0f;
		const float mRecoilKickAmount = 0.1f;
		const float mRecoilPitchAmount = 0.2f;
		const float mRecoilRecoverySpeed = 5.0f;

		// Sway
		const float mSwaySmooth = 8.0f;
		const float mSwayMoveStep = 0.02f;
		const float mMaxSwayMoveStep = 0.12f;
		const float mSwayRotationStep = 0.2f;
		const float mMaxSwayRotationStep = 0.3f;
		glm::vec3 mSwayMoveOffset = glm::vec3(0);
		glm::vec2 mSwayRotationOffset = glm::vec2(0);

		// Bob
		const float mBobSpeed = 13.0f;
		const float mBobAmount = 0.01f;
		const float mBobLerpSpeed = 9.0f;
		float mBobTime = 0.0f;
		float mCurrentBobAmount = 0.0f;
		glm::vec3 mBobMoveOffset = glm::vec3(0);
	};
}