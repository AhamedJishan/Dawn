#pragma once

#include "Core/Scene.h"

namespace Dawn
{
	// Forward declarations
	class EnemySpawner;

	class GameScene : public Scene
	{
	public:
		GameScene();
		~GameScene();

		void Init() override;

		EnemySpawner* GetEnemySpawner() const { return mEnemySpawner; }

	private:
		EnemySpawner* mEnemySpawner = nullptr;
	};
}