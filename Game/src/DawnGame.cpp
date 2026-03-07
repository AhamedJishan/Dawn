#include "DawnGame.h"

#include "Core/Application.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"

namespace Dawn
{
	DawnGame::DawnGame()
	{
	}

	DawnGame::~DawnGame()
	{
	}
	
	void DawnGame::Start()
	{
		//For the final build
		//Application::Get()->LoadScene<MainMenuScene>();
		Application::Get()->LoadScene<GameScene>();
	}
}