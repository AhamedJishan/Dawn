#include "EnemyKamikaze.h"
#include "Utils/Log.h"

#include "Rendering/Mesh.h"
#include "Rendering/Materials/PhongMaterial.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Components/Damageable.h"
#include "PlayerActor.h"

namespace Dawn
{
	EnemyKamikaze::EnemyKamikaze(Scene* scene, PlayerActor* player)
		:Actor(scene)
		,mPlayer(player)
	{
		MeshRenderer::CreateFromModel(this, "Assets/Models/enemy/enemy.obj");
		mCollider = new SphereCollider(this);
		LOG_INFO("Enemy spawned");
		// Fetch the body material
		const std::vector<MeshRenderer*> meshRenderers = GetComponents<MeshRenderer>();
		for (MeshRenderer* meshRenderer : meshRenderers)
		{
			if (meshRenderer->GetMesh()->GetName() == "Body")
			{
				PhongMaterial* mat = dynamic_cast<PhongMaterial*>(meshRenderer->GetMaterial());
				if (mat)
					mBodyMaterial = mat;
			}
		}

		mDamageable = new Damageable(this, 100.0f);
	}

	EnemyKamikaze::~EnemyKamikaze()
	{
		LOG_INFO("Enemy died");
	}

	void EnemyKamikaze::Update(float deltaTime)
	{
		if (mActionState == ActionState::Chasing)
			Chase(deltaTime);
		else if (mActionState == ActionState::Exploding)
			Exploding(deltaTime);
		else
			Explode(deltaTime);


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

	void EnemyKamikaze::TakeDamage(float dmg)
	{
		mDamageable->TakeDamage(dmg);
		if (mDamageable->IsDead())
			SetState(Actor::State::Dead);

		mHitImpactTimer = mHitImpactDuration;

		if (mBodyMaterial)
			mBodyMaterial->SetDiffuseColor(mBodyHitColor);
		mIsInImpactState = true;
	}

	void EnemyKamikaze::Chase(float deltaTime)
	{
		if (!mPlayer)
			return;

		glm::vec3 moveDir = mPlayer->GetPosition() - GetPosition();
		moveDir.y = 0;

		if (glm::length(moveDir) > 1.5f)
		{
			moveDir = glm::normalize(moveDir);
			glm::vec3 newPos = GetPosition() + mSpeed * moveDir * deltaTime;

			SetPosition(newPos);
			SetRotation(glm::quatLookAt(moveDir, GetUp()));
		}
		else
			mActionState = ActionState::Exploding;
	}

	void EnemyKamikaze::Exploding(float deltaTime)
	{
		mExplosionTimer -= deltaTime;
		if (mExplosionTimer <= 0.0f)
			mActionState = ActionState::Explode;
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
		mState = State::Dead;

		if (!mPlayer)
			return;

		float distanceFromPlayer = glm::length(mPlayer->GetPosition() - GetPosition());
		if (distanceFromPlayer >= mExplosionRadius)
			return;

		mPlayer->TakeDamage(mExplosionDamage);
	}
}