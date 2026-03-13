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
		void SyncTransform();

	private:
		PlayerActor* mPlayer = nullptr;
		Audio* mAudioComponent = nullptr;

		float mDamage = 35.0f;
		const float mFireCooldown = 0.1f;
		float mTimeSinceLastFire = 0.0f;

		glm::vec3 mRecoilOffset = glm::vec3(0.0f);
		float mRecoilPitch = 0.0f;

		const float mRecoilKickAmount = 0.08f;
		const float mRecoilPitchKick = 0.5f;
		const float mRecoilRecoverySpeed = 10.0f;
	};
}