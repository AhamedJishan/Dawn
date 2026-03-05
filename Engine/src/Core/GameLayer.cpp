#include "GameLayer.h"

#include "Scene.h"

namespace Dawn
{
	GameLayer::GameLayer()
		:mState(State::Gameplay)
	{
	}

	GameLayer::~GameLayer()
	{
		delete mScene;
	}

	void GameLayer::OnUpdate(float deltaTime)
	{
		if (mScene && mState == State::Gameplay)
			mScene->Update(deltaTime);

		OnGameUpdate(deltaTime);
	}

	void GameLayer::LoadScene(Scene* scene)
	{
		delete mScene;
		mScene = scene;
	}
}
