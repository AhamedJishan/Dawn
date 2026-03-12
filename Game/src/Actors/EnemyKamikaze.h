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

		EnemyKamikaze(Scene* scene, PlayerActor* player);
		~EnemyKamikaze();

		void Update(float deltaTime) override;

		void TakeDamage(float dmg);

	private:
		void Chase(float deltaTime);
		void Exploding(float deltaTime);
		void Explode(float deltaTime);

	private:
		PlayerActor* mPlayer = nullptr;
		SphereCollider* mCollider = nullptr;
		PhongMaterial* mBodyMaterial = nullptr;
		Audio* mAudioComponent = nullptr;
		SoundEvent mEnemyPresence;

		float mExplosionTimer = 1.5f;
		const float mExplosionRadius = 4.0f;
		const float mExplosionDamage = 55.0f;
		const float mScaleAtExplosion = 2.0f;
		const float mExplosionScaleDuration = 0.1f;
		Damageable* mDamageable = nullptr;
		ActionState mActionState = ActionState::Chasing;

		const glm::vec3 mBodyBaseColor = glm::vec3(0.2f);
		const glm::vec3 mBodyHitColor = glm::vec3(0.8f, 0.0f, 0.0f);
		const glm::vec3 mBodyExplosionPulseColor = glm::vec3(1.0f, 0.2f, 0.0f);

		float mSpeed = 4.0f;

		bool mIsInImpactState = false;
		float mHitImpactTimer = 0.0f;
		const float mHitImpactDuration = 0.15f;
		const float mScalePunchAmount = 0.15f;
	};
}