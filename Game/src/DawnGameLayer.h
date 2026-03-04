#pragma once

#include "Core/GameLayer.h"

namespace Dawn
{
	// Forward Declarations
	class EnemySpawner;

	class DawnGameLayer : public GameLayer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnGameUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		void DrawPauseWindow();
		void DrawKillCounter();

	private:
		float mTime = 0;
		int mFrames = 0;

		EnemySpawner* mEnemySpawner = nullptr;
	};
}