#include "EnemySpawner.h"

#include "Core/Scene.h"
#include "Actors/EnemyKamikaze.h"

namespace Dawn
{
	EnemySpawner::EnemySpawner(Scene* scene, PlayerActor* player)
		:Actor(scene)
		,mPlayer(player)
	{
		mSpawnPositions.emplace_back(glm::vec3(30.0f, 0.0f, -30.0f));
		mSpawnPositions.emplace_back(glm::vec3(-30.0f, 0.0f, -30.0f));
		mSpawnPositions.emplace_back(glm::vec3(30.0f, 0.0f, 30.0f));
		mSpawnPositions.emplace_back(glm::vec3(-30.0f, 0.0f, 30.0f));
	}

	void EnemySpawner::Update(float deltaTime)
	{
		// Remove invalid actor + increase mEnemiesKilled
		auto newEnd = std::remove_if(mEnemies.begin(), mEnemies.end(),
			[this](EnemyKamikaze* e)
			{
				return !mScene->ContainsActor(e);
			});
		unsigned int enemiesKilledThisFrame = std::distance(newEnd, mEnemies.end());
		mEnemiesKilled += enemiesKilledThisFrame;
		mEnemies.erase(newEnd, mEnemies.end());

		mTimeSinceLastSpawn += deltaTime;
		if (static_cast<unsigned int>(mEnemies.size()) < mMaxEnemyCount && mTimeSinceLastSpawn >= mSpawnInterval)
		{
			mTimeSinceLastSpawn = 0.0f;
			// Spawn Enemy
			EnemyKamikaze* enemy = new EnemyKamikaze(mScene, mPlayer);
			mEnemies.emplace_back(enemy);

			// Set enemy location
			if (mLastSpawnIndex >= static_cast<unsigned int>(mSpawnPositions.size()))
				mLastSpawnIndex = 0;
			enemy->SetPosition(mSpawnPositions[mLastSpawnIndex]);
			mLastSpawnIndex++;
		}
	}
}