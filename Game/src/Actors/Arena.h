#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"
#include "Rendering/Materials/PhongMaterial.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class Arena : public Actor
	{
	public:
		Arena(Scene* scene, Actor* player)
			:Actor(scene)
		{
			mPlayer = player;
			MeshRenderer::CreateFromModel(this, "Assets/Models/arena02/arena.obj");
		}

		void Update(float deltaTime) override
		{
			if (!mPlayer)
				return;

			glm::vec3 playerPos = mPlayer->GetPosition();
	
			if (playerPos.x > mBounds.x)
				playerPos.x = mBounds.x;
			if (playerPos.x < -mBounds.x)
				playerPos.x = -mBounds.x;
			if (playerPos.z > mBounds.y)
				playerPos.z = mBounds.y;
			if (playerPos.z < -mBounds.y)
				playerPos.z = -mBounds.y;

			mPlayer->SetPosition(playerPos);
		}

	private:
		Actor* mPlayer = nullptr;
		glm::vec2 mBounds = glm::vec2(25.0f);
	};
}