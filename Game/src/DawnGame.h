#pragma once

#include "Core/Game.h"
#include <imgui/imgui.h>

namespace Dawn
{
	enum class GameState
	{
		MainMenu,
		Gameplay,
		Paused
	};

	class DawnGame : public Game
	{
	public:
		DawnGame();
		~DawnGame();

		void Start() override;

	private:
		float mTime = 0;
		int mFrames = 0;
	};
}