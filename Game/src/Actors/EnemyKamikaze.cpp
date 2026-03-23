#include "EnemyKamikaze.h"
#include "Utils/Log.h"

#include <stdlib.h>
#include <time.h>
#include "Rendering/Mesh.h"
#include "Rendering/Materials/PhongMaterial.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Core/Components/Audio.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Components/Damageable.h"
#include "WaveManager.h"
#include "Player.h"

namespace Dawn
{
	EnemyKamikaze::EnemyKamikaze(Scene* scene, Player* player, WaveManager* waveManager)
		:Actor(scene)
		,mPlayer(player)
		,mWaveManager(waveManager)
	{
		LOG_INFO("Enemy spawned");

		MeshRenderer::CreateFromModel(this, "Assets/Models/enemy/enemy.obj");
		mCollider = new SphereCollider(this);
		mCollider->SetIsDynamic(true);
		mCollider->SetIsTrigger(false);

		// Fetch the body material
		const std::vector<MeshRenderer*> meshRenderers = GetComponents<MeshRenderer>();
		for (MeshRenderer* meshRenderer : meshRenderers)
		{
			if (meshRenderer->GetMesh()->GetName() == "Body")
			{
				PhongMaterial* mat = dynamic_cast<PhongMaterial*>(meshRenderer->GetMaterial());
				if (mat)
				{
					mBodyMaterial = mat;
					mBodyBaseColor = mat->GetDiffuseColor();
				}
			}
		}

		mDamageable = new Damageable(this, 90.0f);

		mAudioComponent = new Audio(this);
		mEnemyPresence = mAudioComponent->PlayEvent("event:/enemy_presence");
		mEnemyPresence.SetVolume(10.0f);

		mMoveDirection = glm::normalize(mPlayer->GetPosition() - GetPosition());

		srand(time(0));
		float variation = ((rand() % 10) / 10.0f) * mSpeedVariation;
		mSpeed += variation;
	}

	EnemyKamikaze::~EnemyKamikaze()
	{
		LOG_INFO("Enemy died");
	}

	void EnemyKamikaze::Update(float deltaTime)
	{
		if (mActionState == ActionState::Chasing)
		{
			Chase(deltaTime);
		}
		else if (mActionState == ActionState::Exploding)
		{
			Exploding(deltaTime);
			mEnemyPresence.Stop();
		}
		else
		{
			Explode(deltaTime);
		}


		// --- FEEDBACK STUFF ---
		if (mActionState == ActionState::Chasing && mHitImpactTimer > 0.0f)
		{
			mHitImpactTimer -= deltaTime;
			float t = 1 - mHitImpactTimer / mHitImpactDuration;
			float scaleOffset = mScalePunchAmount * glm::sin(t * glm::pi<float>());
			SetScale(glm::vec3(1.0f + scaleOffset));
		}
		else if (mIsInImpactState)
		{
			mIsInImpactState = false;
			if (mBodyMaterial)
				mBodyMaterial->SetDiffuseColor(mBodyBaseColor);
		}

	}

	float EnemyKamikaze::TakeDamage(float dmg)
	{
		float health = mDamageable->TakeDamage(dmg);

		if (mDamageable->IsDead())
			SetState(Actor::State::Dead);

		mHitImpactTimer = mHitImpactDuration;

		if (mBodyMaterial)
			mBodyMaterial->SetDiffuseColor(mBodyHitColor);
		mIsInImpactState = true;

		return health;
	}

	void EnemyKamikaze::Chase(float deltaTime)
	{
		if (!mPlayer)
			return;

		float distanceToPlayer = glm::length(mPlayer->GetPosition() - GetPosition());

		glm::vec3 targetMoveDirection = GetFinalMoveDirection();
		glm::vec3 moveDir = glm::mix(mMoveDirection, targetMoveDirection, mMoveDirectionSmoothing * deltaTime);
		mMoveDirection = moveDir;
		moveDir.y = 0;

		if (distanceToPlayer < mExplosionTriggerRadius)
		{
			mAudioComponent->PlayEvent("event:/enemy_fuse");
			mActionState = ActionState::Exploding;
		}
		else
		{
			moveDir = glm::normalize(moveDir);
			glm::vec3 newPos = GetPosition() + mSpeed * moveDir * deltaTime;

			SetPosition(newPos);
			SetRotation(glm::quatLookAt(moveDir, GetUp()));
		}
	}

	void EnemyKamikaze::Exploding(float deltaTime)
	{
		mExplosionTimer -= deltaTime;
		if (mExplosionTimer <= 0.0f)
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/enemy_explode", GetPosition());
			mActionState = ActionState::Explode;
		}
		else if (mExplosionTimer <= mExplosionScaleDuration)
		{
			// linearly interpolate to scale
			float t = 1 - mExplosionTimer / mExplosionScaleDuration;
			SetScale(glm::vec3(1 + t * (mScaleAtExplosion - 1.0f)));
		}

		if (!mBodyMaterial)
			return;

		bool pulse = (static_cast<int>(3 * mExplosionTimer) % 2) == 0;
		if (pulse)
			mBodyMaterial->SetDiffuseColor(mBodyExplosionPulseColor);
		else
			mBodyMaterial->SetDiffuseColor(mBodyBaseColor);
	}

	void EnemyKamikaze::Explode(float deltaTime)
	{
		SetState(Actor::State::Dead);

		if (!mPlayer)
			return;

		float distanceFromPlayer = glm::length(mPlayer->GetPosition() - GetPosition());
		if (distanceFromPlayer >= mExplosionRadius)
			return;

		mPlayer->TakeDamage(mExplosionDamage);
	}

	glm::vec3 EnemyKamikaze::GetFinalMoveDirection()
	{
		EnemyKamikaze* closestEnemy = nullptr;
		float closestDistanceToEnemy = std::numeric_limits<float>::max();
		for (EnemyKamikaze* enemy : mWaveManager->GetEnemies())
		{
			float distanceToEnemy = glm::length(enemy->GetPosition() - GetPosition());
			if (distanceToEnemy < closestDistanceToEnemy && enemy != this)
			{
				closestDistanceToEnemy = distanceToEnemy;
				closestEnemy = enemy;
			}
		}

		glm::vec3 directionToPlayer = glm::normalize(mPlayer->GetPosition() - GetPosition());
		if (closestEnemy == nullptr || closestDistanceToEnemy > mSteeringRange)
			return glm::normalize(directionToPlayer);

		glm::vec3 directionAwayFromClosestEnemy = glm::normalize(GetPosition() - closestEnemy->GetPosition());
		if (closestDistanceToEnemy <= mSteeringCutOffRange)
			return glm::normalize(directionAwayFromClosestEnemy);

		float distanceFactor = glm::clamp((closestDistanceToEnemy / mSteeringRange), 0.0f, 1.0f);
		glm::vec3 direction = distanceFactor * directionToPlayer + (1 - distanceFactor) * directionAwayFromClosestEnemy;
		return glm::normalize(direction);
	}
}