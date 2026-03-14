#pragma once

#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "EnemyKamikaze.h"

namespace Dawn
{
	class Projectile : public Actor
	{
	public:
		Projectile(class Scene* scene, float damage = 35.0f)
			:Actor(scene)
			,mDamage(damage)
		{
			mCollider = new SphereCollider(this);
			mCollider->SetRadius(0.2f);
			MeshRenderer::CreateFromModel(this, "Assets/Models/ball/ball.obj");
			SetScale(glm::vec3(0.2f));
		}

		void Update(float deltaTime) override
		{
			mLifeTime -= deltaTime;
			if (mLifeTime <= 0.0f)
				SetState(Actor::State::Dead);

			SetPosition(GetPosition() + GetForward() * mSpeed * deltaTime);

			// Check for collisions
			EnemyKamikaze* enemy = dynamic_cast<EnemyKamikaze*>(mCollider->CheckCollisions());
			if (enemy)
			{
				SetState(Actor::State::Dead);
				enemy->TakeDamage(mDamage);
			}
		}

	private:
		float mLifeTime = 1.0f;
		float mSpeed = 40.0f;
		float mDamage = 35.0f;

		SphereCollider* mCollider = nullptr;
	};
}