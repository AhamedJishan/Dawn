#include "WaveManager.h"
#include "Utils/Log.h"

#include "Core/Scene.h"
#include "Player.h"
#include "EnemyKamikaze.h"

namespace Dawn
{
	WaveManager::WaveManager(Scene* scene, Player* player)
		:Actor(scene)
		,mPlayer(player)
	{
		Init();
	}
	WaveManager::~WaveManager()
	{
	}

	void WaveManager::Init()
	{
		// === WAVE 1 ===
		Wave wave1;
		wave1.phases.push_back(Phase(0.0f, 5, 0));
		wave1.phases.push_back(Phase(7.5f, 5, 2));

		// === WAVE 2 ===
		Wave wave2;
		wave2.phases.push_back(Phase(0.0f, 5, 1));
		wave2.phases.push_back(Phase(5.0f, 5, 3));
		wave2.phases.push_back(Phase(0.0f, 5, 2));

		mWaves.push_back(wave1);
		mWaves.push_back(wave2);

		mWaveState = WaveState::Begin;
	}
	
	void WaveManager::Update(float deltaTime)
	{
		switch (mWaveState)
		{
			case Dawn::WaveState::Begin:
			{
				Begin();
				break;
			}
			case Dawn::WaveState::WaveStarting:
			{
				mWaveTimer += deltaTime;
				if (mWaveTimer >= mTimeBetweenWaves)
				{
					mWaveState = WaveState::WaveActive;

					mWaveTimer = 0.0f;
					mPhaseTimer = 0.0f;

					mCurrentPhaseIndex = 0;
					mWaveEnemiesRemaining = GetTotalEnemyCountForWave(mCurrentWaveIndex);
					mWaveEnemiesDied = 0;
					mEnemies.clear();
				}
				break;
			}
			case Dawn::WaveState::WaveActive:
			{
				mPhaseTimer += deltaTime;

				UpdateWaveEnemyState();

				const Wave& currentWave = mWaves[mCurrentWaveIndex];

				if (mCurrentPhaseIndex < currentWave.phases.size())
				{
					const Phase& currentPhase = currentWave.phases[mCurrentPhaseIndex];

					if (mPhaseTimer >= currentPhase.triggerTime)
					{
						mPhaseTimer = 0.0f;
						QueueSpawn(currentPhase.enemyCount, currentPhase.spawnLocationIndex);
						mCurrentPhaseIndex++;
					}
				}

				for (unsigned int spawnLocationIndex = 0; spawnLocationIndex < NUM_SPAWN_LOCATIONS; spawnLocationIndex++)
				{
					mTimeSinceLastSpawn[spawnLocationIndex] += deltaTime;

					if ((mSpawnQueue[spawnLocationIndex] > 0) && (mTimeSinceLastSpawn[spawnLocationIndex] >= mTimeBetweenSpawns))
					{
						mSpawnQueue[spawnLocationIndex] -= 1;
						mTimeSinceLastSpawn[spawnLocationIndex] = 0.0f;
						EnemyKamikaze* enemy = new EnemyKamikaze(mScene, mPlayer, this);
						enemy->SetPosition(mSpawnLocations[spawnLocationIndex]);
						mEnemies.push_back(enemy);
					}
				}
				
				if (mCurrentPhaseIndex >= currentWave.phases.size() && mWaveEnemiesRemaining == 0)
					mWaveState = WaveState::WaveClear;

				break;
			}
			case Dawn::WaveState::WaveClear:
			{
				mCurrentWaveIndex++;
				mCurrentPhaseIndex = 0;

				mWaveState = WaveState::WaveStarting;
				mSpawnQueue.fill(0);
				mTimeSinceLastSpawn.fill(0.0f);
				mWaveTimer = 0.0f;

				if (mCurrentWaveIndex >= mWaves.size())
					mWaveState = WaveState::End;

				break;
			}
			case Dawn::WaveState::End:
			{
				// TODO: Victory
				break;
			}
			default:
			{
				// Do nothing
				break;
			}
		}
	}

	void WaveManager::QueueSpawn(unsigned int enemyCount, unsigned int spawnLocationIndex)
	{
		if (spawnLocationIndex >= NUM_SPAWN_LOCATIONS)
		{
			LOG_WARN("Spawn location index '%d', is out of range!", spawnLocationIndex);
			return;
		}

		mSpawnQueue[spawnLocationIndex] += enemyCount;
		mTimeSinceLastSpawn[spawnLocationIndex] = 0.0f;
	}

	unsigned int WaveManager::GetTotalEnemyCountForWave(unsigned int waveIndex)
	{
		if (waveIndex >= mWaves.size())
		{
			LOG_WARN("Wave index '%d', is out of range!", waveIndex);
			return 0;
		}

		unsigned int enemyCount = 0;

		for (const Phase& phase : mWaves[waveIndex].phases)
			enemyCount += phase.enemyCount;

		return enemyCount;
	}

	void WaveManager::Begin()
	{
		mWaveState = WaveState::WaveStarting;

		mSpawnQueue.fill(0);
		mTimeSinceLastSpawn.fill(0.0f);

		mWaveTimer = 0.0f;
		mPhaseTimer = 0.0f;

		mCurrentWaveIndex = 0;
		mCurrentPhaseIndex = 0;

		mWaveEnemiesRemaining = 0;
		mWaveEnemiesDied = 0;

		mEnemies.clear();
	}

	void WaveManager::UpdateWaveEnemyState()
	{
		auto newEnd = std::remove_if(mEnemies.begin(), mEnemies.end(),
			[this](EnemyKamikaze* e)
			{
				return !mScene->ContainsActor(e);
			});
		unsigned int enemiesKilledThisFrame = std::distance(newEnd, mEnemies.end());
		mEnemies.erase(newEnd, mEnemies.end());

		mWaveEnemiesDied += enemiesKilledThisFrame;
		mWaveEnemiesRemaining = GetTotalEnemyCountForWave(mCurrentWaveIndex) - mWaveEnemiesDied;
	}
}