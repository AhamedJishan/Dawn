#pragma once
#include "Core/Actor.h"
#include "Utils/Log.h"

#include <vector>
#include <glm/vec3.hpp>
#include "Actors/Enemy.h"
#include "Actors/PlayerActor.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class EnemySpawner : public Actor
	{
	public:
		EnemySpawner(Scene* scene, PlayerActor* player)
			:Actor(scene)
			,mPlayer(player)
		{
			mSpawnPositions.emplace_back(glm::vec3( 20.0f, 0.0f, -20.0f));
			mSpawnPositions.emplace_back(glm::vec3(-20.0f, 0.0f, -20.0f));
			mSpawnPositions.emplace_back(glm::vec3( 20.0f, 0.0f,  20.0f));
			mSpawnPositions.emplace_back(glm::vec3(-20.0f, 0.0f,  20.0f));
		}

		void Update(float deltaTime) override
		{
			mTimeSinceLastSpawn += deltaTime;
			if (static_cast<unsigned int>(mEnemies.size()) < mMaxEnemyCount && mTimeSinceLastSpawn >= mSpawnInterval)
			{
				mTimeSinceLastSpawn = 0.0f;
				// Spawn Enemy
				Enemy* enemy = new Enemy(mScene, mPlayer);
				mEnemies.emplace_back(enemy);

				// Set enemy location
				if (mLastSpawnIndex >= static_cast<unsigned int>(mSpawnPositions.size()))
					mLastSpawnIndex = 0;
				enemy->SetPosition(mSpawnPositions[mLastSpawnIndex]);
				mLastSpawnIndex++;

				LOG_INFO("Enemy spawned");
			}
		}

	private:
		const unsigned int mMaxEnemyCount = 5;
		const float mSpawnInterval = 2.0f;
		float mTimeSinceLastSpawn = 0.0f;

		unsigned int mLastSpawnIndex = 0;

		PlayerActor* mPlayer = nullptr;
		std::vector<Enemy*> mEnemies;
		std::vector<glm::vec3> mSpawnPositions;
	};
}