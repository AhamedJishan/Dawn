#pragma once

#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"

namespace Dawn
{
	class Projectile : public Actor
	{
	public:
		Projectile(class Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/ball/ball.obj");
		}

		void Update(float deltaTime) override
		{
			mLifeTime -= deltaTime;
			if (mLifeTime <= 0.0f)
				SetState(Actor::State::Dead);

			SetPosition(GetPosition() + GetForward() * mSpeed * deltaTime);
		}

	private:
		float mLifeTime = 3.0f;
		float mSpeed = 50.0f;
	};
}