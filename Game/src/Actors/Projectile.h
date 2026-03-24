#pragma once

#include "Core/Actor.h"

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

			mDesc.initialBurst = 10;
			mDesc.particleLifetime = 0.05f;
			mDesc.directionMin = glm::vec3(-1.0f, -1.0f, -1.0f);
			mDesc.directionMax = glm::vec3(1.0f, 1.0f, 1.0f);
			mDesc.speed = 60.0f;
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

				new ParticleSystem(mScene, mDesc, GetPosition());
			}

			if (mArena->IsOutOfBounds(GetPosition()))
			{
				SetState(Actor::State::Dead);
				mPlayerKillStreak->ShotMissed();
				new ParticleSystem(mScene, mDesc, GetPosition());
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

		ParticleSystemDesc mDesc;
	};
}