#pragma once

#include "Core/Actor.h"
#include "Utils/Log.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Rendering/Mesh.h"
#include "Rendering/Materials/PhongMaterial.h"
#include "Components/Damageable.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class Enemy : public Actor
	{
	public:
		Enemy(Scene* scene, Actor* chaseTarget)
			:Actor(scene)
			,mChaseTarget(chaseTarget)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/enemy/enemy.obj");
			mCollider = new SphereCollider(this);
			LOG_INFO("Enemy spawned");

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

		~Enemy()
		{
			LOG_INFO("Enemy died");
		}

		void Update(float deltaTime) override
		{
			if (mChaseTarget)
			{
				glm::vec3 moveDir = mChaseTarget->GetPosition() - GetPosition();
				moveDir.y = 0;

				if (glm::length(moveDir) > 1.5f)
				{
					moveDir = glm::normalize(moveDir);

					glm::vec3 newPos = GetPosition() + mSpeed * moveDir * deltaTime;

					SetPosition(newPos);

					SetRotation(glm::quatLookAt(moveDir, GetUp()));
				}
			}

			if (mHitImpactTimer > 0.0f)
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

		void TakeDamage(float dmg)
		{
			mDamageable->TakeDamage(dmg);
			if (mDamageable->IsDead())
				SetState(Actor::State::Dead);

			mHitImpactTimer = mHitImpactDuration;

			if (mBodyMaterial)
				mBodyMaterial->SetDiffuseColor(mBodyHitColor);
			mIsInImpactState = true;
		}

	private:
		Actor* mChaseTarget = nullptr;
		SphereCollider* mCollider = nullptr;
		PhongMaterial* mBodyMaterial = nullptr;

		Damageable* mDamageable = nullptr;

		const glm::vec3 mBodyBaseColor = glm::vec3(0.2f);
		const glm::vec3 mBodyHitColor = glm::vec3(0.9f, 0.3f, 0.3f);

		float mSpeed = 4.0f;

		bool mIsInImpactState = false;
		const float mScalePunchAmount = 0.15f;
		const float mHitImpactDuration = 0.15f;
		float mHitImpactTimer = 0.0f;
	};
}