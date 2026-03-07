#pragma once

#include <imgui/imgui.h>
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
		void Update(float deltaTime) override;
		void ImGuiRender() override;

	private:
		void DrawPauseWindow();
		void DrawKillCounter();

	private:
		EnemySpawner* mEnemySpawner = nullptr;

		ImFont* mFontBold = nullptr;
		ImFont* mFontRegular = nullptr;
		ImFont* mFontLight = nullptr;
	};
}