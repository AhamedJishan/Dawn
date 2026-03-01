#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"

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
				if (glm::length(moveDir) < 1.5f)
					return;
				moveDir = glm::normalize(moveDir);

				glm::vec3 newPos = GetPosition() + mSpeed * moveDir * deltaTime;

				SetPosition(newPos);

				SetRotation(glm::quatLookAt(moveDir, GetUp()));
			}
		}

		void TakeDamage()
		{
			mHealth--;
			if (mHealth <= 0.0f)
				SetState(Actor::State::Dead);
		}

	private:
		Actor* mChaseTarget = nullptr;
		SphereCollider* mCollider = nullptr;

		float mSpeed = 4.0f;
		float mHealth = 3.0f;
	};
}