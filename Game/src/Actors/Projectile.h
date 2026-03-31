#pragma once

#include "Core/Actor.h"

#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Rendering/ParticleSystem.h"
#include "Components/KillStreak.h"
#include "Player.h"
#include "Arena.h"
#include "EnemyKamikaze.h"

namespace Dawn
{
	class Projectile : public Actor
	{
	public:
		Projectile(class Scene* scene, Player* player, float damage = 30.0f)
			:Actor(scene)
			,mDamage(damage)
		{
			mCollider = new SphereCollider(this);
			mCollider->SetIsTrigger(true);
			mCollider->SetRadius(0.3f);

			MeshRenderer::CreateFromModel(this, "Assets/Models/ball/ball.obj");
			SetScale(glm::vec3(0.3f));

			mPlayer = player;
			mArena = player->GetArena();
			mPlayerKillStreak = mPlayer->GetComponent<KillStreak>();

			mHitParticleDesc.initialBurst = 10;
			mHitParticleDesc.emissionRate = 0.0f;
			mHitParticleDesc.particleLifetime = 0.1f;
			mHitParticleDesc.directionMin = glm::vec3(-1.0f, -1.0f, -1.0f);
			mHitParticleDesc.directionMax = glm::vec3(1.0f, 1.0f, 1.0f);
			mHitParticleDesc.speed = 10.0f;
			mHitParticleDesc.scaleOverTime.AddKey(0.0f, glm::vec3(0.3f));
			mHitParticleDesc.scaleOverTime.AddKey(1.0f, glm::vec3(0.1f));
			mHitParticleDesc.colorOverTime.AddKey(0.0f, glm::vec4(10.0f, 9.0f, 5.0f, 1.0f));
			mHitParticleDesc.colorOverTime.AddKey(1.0f, glm::vec4(2.0f, 1.0f, 0.3f, 1.0f));
		}

		void Update(float deltaTime) override
		{
			mLifeTime -= deltaTime;
			if (mLifeTime <= 0.0f)
			{
				SetState(Actor::State::Dead);
				mPlayerKillStreak->ShotMissed();
			}

			SetPosition(GetPosition() + GetForward() * mSpeed * deltaTime);

			// Check for collisions
			EnemyKamikaze* enemy = dynamic_cast<EnemyKamikaze*>(mCollider->CheckCollisions());
			if (enemy)
			{
				SetState(Actor::State::Dead);
				float enemyHealth = enemy->TakeDamage(mDamage);
				if (enemyHealth <= 0.0f)
					mPlayerKillStreak->EnemyKilled();

				glm::vec3 hitDirection = glm::normalize(mPlayer->GetPosition() - enemy->GetPosition());
				mHitParticleDesc.directionMin = hitDirection - glm::vec3(2);
				mHitParticleDesc.directionMax = hitDirection + glm::vec3(2);
				new ParticleSystem(mScene, mHitParticleDesc, GetPosition());

				Application::Get()->GetAudioSystem()->PlayEvent("event:/gunshot_impact");
			}

			if (mArena->IsOutOfBounds(GetPosition()))
			{
				SetState(Actor::State::Dead);
				mPlayerKillStreak->ShotMissed();

				mHitParticleDesc.directionMin = glm::vec3(-1.0f, -1.0f, -1.0f);
				mHitParticleDesc.directionMax = glm::vec3(1.0f, 1.0f, 1.0f);
				new ParticleSystem(mScene, mHitParticleDesc, GetPosition());
			}
		}

	private:
		float mLifeTime = 1.0f;
		float mSpeed = 100.0f;
		float mDamage = 35.0f;

		SphereCollider* mCollider = nullptr;
		Player* mPlayer = nullptr;
		Arena* mArena = nullptr;
		KillStreak* mPlayerKillStreak = nullptr;

		ParticleSystemDesc mHitParticleDesc;
	};
}