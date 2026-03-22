#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Audio/SoundEvent.h"

namespace Dawn
{
	// Forward declarations
	class Scene;
	class Damageable;
	class PhongMaterial;
	class SphereCollider;
	class WaveManager;
	class PlayerActor;
	class Audio;

	class EnemyKamikaze : public Actor
	{
	public:
		enum class ActionState
		{
			Chasing,
			Exploding,
			Explode
		};

		EnemyKamikaze(Scene* scene, PlayerActor* player, WaveManager* waveManager);
		~EnemyKamikaze();

		void Update(float deltaTime) override;

		void TakeDamage(float dmg);

	private:
		void Chase(float deltaTime);
		void Exploding(float deltaTime);
		void Explode(float deltaTime);
		glm::vec3 GetFinalMoveDirection();

	private:
		WaveManager* mWaveManager = nullptr;
		PlayerActor* mPlayer = nullptr;
		SphereCollider* mCollider = nullptr;
		PhongMaterial* mBodyMaterial = nullptr;
		Audio* mAudioComponent = nullptr;
		SoundEvent mEnemyPresence;

		float mExplosionTimer = 1.5f;
		const float mExplosionTriggerRadius = 1.5f;
		const float mExplosionRadius = 5.0f;
		const float mExplosionDamage = 55.0f;
		const float mScaleAtExplosion = 2.0f;
		const float mExplosionScaleDuration = 0.1f;
		Damageable* mDamageable = nullptr;
		ActionState mActionState = ActionState::Chasing;

		glm::vec3 mBodyBaseColor;
		const glm::vec3 mBodyHitColor = glm::vec3(0.4f, 0.0f, 0.0f);
		const glm::vec3 mBodyExplosionPulseColor = glm::vec3(1.0f, 0.2f, 0.0f);

		float mSpeed = 4.0f;
		const float mSpeedVariation = 2.0f;
		const float mSteeringRange = 5.0f;
		const float mSteeringCutOffRange = 2.0f;
		const float mMoveDirectionSmoothing = 2.5f;
		glm::vec3 mMoveDirection = glm::vec3(0);

		bool mIsInImpactState = false;
		float mHitImpactTimer = 0.0f;
		const float mHitImpactDuration = 0.15f;
		const float mScalePunchAmount = 0.15f;
	};
}