#pragma once

#include "Core/Game.h"
#include <imgui/imgui.h>

namespace Dawn
{
	enum class GameState
	{
		Gameplay,
		Paused
	};

	class DawnGame : public Game
	{
	public:
		DawnGame();
		~DawnGame();

		void Start() override;
		void Update(float deltaTime) override;
		void ImGuiRender() override;

	private:
		void DrawPauseWindow();
		void DrawKillCounter();

	private:
		float mTime = 0;
		int mFrames = 0;

		GameState mState;

		ImFont* mRobotoRegular = nullptr;
		ImFont* mRobotoBold = nullptr;
	};
}