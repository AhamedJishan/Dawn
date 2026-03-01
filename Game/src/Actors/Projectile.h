#pragma once

#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Enemy.h"

namespace Dawn
{
	class Projectile : public Actor
	{
	public:
		Projectile(class Scene* scene)
			:Actor(scene)
		{
			mCollider = new SphereCollider(this);
			mCollider->SetRadius(0.2f);
			MeshRenderer::CreateFromModel(this, "Assets/Models/ball/ball.obj");
		}

		void Update(float deltaTime) override
		{
			mLifeTime -= deltaTime;
			if (mLifeTime <= 0.0f)
				SetState(Actor::State::Dead);

			SetPosition(GetPosition() + GetForward() * mSpeed * deltaTime);

			// Check for collisions
			Enemy* enemy = dynamic_cast<Enemy*>(mCollider->CheckCollisions());
			if (enemy)
			{
				SetState(Actor::State::Dead);
				enemy->TakeDamage();
			}
		}

	private:
		float mLifeTime = 1.0f;
		float mSpeed = 100.0f;

		SphereCollider* mCollider = nullptr;
	};
}