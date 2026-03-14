#pragma once

#include "Core/Actor.h"
#include "glm/vec3.hpp"

namespace Dawn
{
	// Forward declarations
	class PlayerActor;
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

	private:
		PlayerActor* mPlayer = nullptr;
		Audio* mAudioComponent = nullptr;

		float mDamage = 35.0f;
		const float mFireCooldown = 0.1f;
		float mTimeSinceLastFire = 0.0f;

		// Recoil
		glm::vec3 mRecoilOffset = glm::vec3(0.0f);
		float mRecoilPitch = 0.0f;
		const float mRecoilKickAmount = 0.1f;
		const float mRecoilPitchAmount = 0.2f;
		const float mRecoilRecoverySpeed = 5.0f;

		// Sway
		const float mSwaySmooth = 12.0f;
		const float mSwayMoveStep = 0.01f;
		const float mMaxSwayMoveStep = 0.06f;
		const float mSwayRotationStep = 0.2f;
		const float mMaxSwayRotationStep = 0.3f;

		glm::vec3 mSwayMoveOffset = glm::vec3(0.0f);
		glm::vec2 mSwayRotationOffset = glm::vec2(0.0f);
	};
}