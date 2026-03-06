#include "DawnGame.h"

#include "Core/Application.h"
#include "Scenes/MainMenuScene.h"

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
		Application::Get()->LoadScene<MainMenuScene>();
	}
}