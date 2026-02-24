#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"
#include "Rendering/Materials/PhongMaterial.h"

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
		}

		void Update(float deltaTime) override
		{
			if (mChaseTarget)
			{
				glm::vec3 moveDir = mChaseTarget->GetPosition() - GetPosition();
				moveDir.y = 0;
				moveDir = glm::normalize(moveDir);

				glm::vec3 newPos = GetPosition() + mSpeed * moveDir * deltaTime;

				SetPosition(newPos);

				SetRotation(glm::quatLookAt(moveDir, GetUp()));
			}
		}

	private:
		Actor* mChaseTarget = nullptr;

		float mSpeed = 2.5f;
	};
}