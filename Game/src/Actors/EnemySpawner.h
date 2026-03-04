#pragma once
#include "Core/Actor.h"

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
		EnemySpawner(Scene* scene, PlayerActor* player);

		void Update(float deltaTime) override;

		unsigned int GetEnemiesKilled() const { return mEnemiesKilled; }

	private:
		unsigned int mEnemiesKilled = 0;

		const unsigned int mMaxEnemyCount = 5;
		const float mSpawnInterval = 2.0f;
		float mTimeSinceLastSpawn = 0.0f;

		unsigned int mLastSpawnIndex = 0;

		PlayerActor* mPlayer = nullptr;
		std::vector<Enemy*> mEnemies;
		std::vector<glm::vec3> mSpawnPositions;
	};
}