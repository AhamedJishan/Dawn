#pragma once

#include "Utils/Log.h"
#include "Core/Scene.h"
#include "Core/GameLayer.h"

#include <Vendor/imgui/imgui.h>

#include "Input/Input.h"
#include "Core/Components/Camera.h"
#include "ExampleActor.h"
#include "Actors/FPSCameraActor.h"
#include "Actors/PlayerActor.h"
#include "Actors/GroundPlane.h"
#include "Actors/EnemySpawner.h"

namespace Dawn
{
	class ExampleGameLayer : public GameLayer
	{
	public:
		void OnAttach() override
		{
			LOG_WARN("Example Layer attached!");

			// Right now creating the scene manually
			// TODO: via serialization, load scene object
			Scene* scene = new Scene("Assets/Scenes/scene.scene");
			LoadScene(scene);

			Input::SetCursorLocked(true);

			// TEST ACTOR
			ExampleActor* ea = new ExampleActor(GetScene());
			ea->SetPosition(glm::vec3(5, 0, -5));
			ea->SetScale(glm::vec3(0.6f));

			// GROUND
			GroundPlane* ground = new GroundPlane(mScene);
			// PLAYER
			FPSCameraActor* cameraActor = new FPSCameraActor(mScene);
			PlayerActor* player = new PlayerActor(mScene, cameraActor);
			player->SetPosition(glm::vec3(0, 0, 4));
			// ENEMY SPAWNER
			EnemySpawner* enemySpawner = new EnemySpawner(mScene, player);
			
			Camera* cam = cameraActor->GetComponent<Camera>();
			if (cam)
			{
				GetScene()->SetActiveCamera(cam);
			}
		}

		void OnDetach() override
		{
			LOG_WARN("Example Layer detached!");
		}

		void OnGameUpdate(float deltaTime) override
		{
			if (Input::GetKeyDown(Key::Escape))
			{
				if (mState == State::Gameplay)
				{
					// Pause the game
					mState = State::Paused;
					Input::SetCursorLocked(false);

				}
				else if (mState == State::Paused)
				{
					// Resume the game
					mState = State::Gameplay;
					Input::SetCursorLocked(true);
				}
			}
		}

		void OnImGuiRender() override
		{
			if (mState == State::Paused)
				ImGui::ShowDemoWindow();
		}

	private:
		float mTime = 0;
		int mFrames = 0;
	};
}