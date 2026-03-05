#include "GameScene.h"

#include "ExampleActor.h"
#include "Actors/GroundPlane.h"
#include "Actors/EnemySpawner.h"
#include "Actors/PlayerActor.h"
#include "Actors/FPSCameraActor.h"
#include "Core/Components/Camera.h"

namespace Dawn
{
	GameScene::GameScene()
	{
	}

	GameScene::~GameScene()
	{
	}
	
	void GameScene::Init()
	{
		// TEST ACTOR
		ExampleActor* ea = new ExampleActor(this);
		ea->SetPosition(glm::vec3(5, 0, -5));
		ea->SetScale(glm::vec3(0.6f));

		// GROUND
		GroundPlane* ground = new GroundPlane(this);
		// PLAYER
		FPSCameraActor* cameraActor = new FPSCameraActor(this);
		PlayerActor* player = new PlayerActor(this, cameraActor);
		player->SetPosition(glm::vec3(0, 0, 4));
		// ENEMY SPAWNER
		mEnemySpawner = new EnemySpawner(this, player);

		Camera* cam = cameraActor->GetComponent<Camera>();
		if (cam)
		{
			SetActiveCamera(cam);
		}
	}
}