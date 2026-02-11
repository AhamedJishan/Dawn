#include "GameLayer.h"

#include "Scene.h"

namespace Dawn
{
	GameLayer::GameLayer()
	{
	}
	GameLayer::~GameLayer()
	{
		delete mScene;
	}

	void GameLayer::OnUpdate(float deltaTime)
	{
		if (mScene)
			mScene->Update(deltaTime);

		OnGameUpdate(deltaTime);
	}

	void GameLayer::LoadScene(Scene* scene)
	{
		delete mScene;
		mScene = scene;
	}
}
