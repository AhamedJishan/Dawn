#pragma once

#include "Core/Actor.h"
#include "glm/vec3.hpp"
#include "Rendering/ParticleSystem.h"

namespace Dawn
{
	// Forward declarations
	class Player;
	class Scene;
	class Audio;
	class PhongMaterial;

	class Gun : public Actor
	{
	public:
		Gun(Scene* scene, Player* player);

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
		Player* mPlayer = nullptr;
		Audio* mAudioComponent = nullptr;
		PhongMaterial* mMaterial = nullptr;

		// Damage
		const float mBaseDamage = 30.0f;
		const float mBonusDamage = 30.0f;
		unsigned int mBonusDmgMultiplier = 0;
		static constexpr unsigned int mMaxDmgMultiplier = 2;

		glm::vec4 mChargeColors[mMaxDmgMultiplier + 1] =
		{
			glm::vec4(0.40f, 0.80f, 0.85f, 1.0f), // Level 0 (teal)
			glm::vec4(0.75f, 0.10f, 0.65f, 1.0f), // Level 1 (purple)
			glm::vec4(0.85f, 0.10f, 0.04f, 1.0f)  // Level 2 (orange)
		};

		const float mFireCooldown = 0.1f;
		float mTimeSinceLastFire = 0.0f;

		glm::vec3 mProjectileSpawnOffset = glm::vec3(0.0f, 0.05f, 0.15f);
		ParticleSystemDesc mMuzzleFlashDesc;

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