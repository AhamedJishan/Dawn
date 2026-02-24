#pragma once

#include "Utils/Log.h"
#include "Core/Scene.h"
#include "Core/GameLayer.h"

#include "Input/Input.h"
#include "Core/Components/Camera.h"
#include "ExampleActor.h"
#include "Actors/FPSCameraActor.h"
#include "Actors/PlayerActor.h"
#include "Actors/GroundPlane.h"

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

			GroundPlane* ground = new GroundPlane(mScene);

			ExampleActor* ea = new ExampleActor(GetScene());
			ea->SetScale(glm::vec3(0.6f));

			FPSCameraActor* cameraActor = new FPSCameraActor(mScene);
			PlayerActor* player = new PlayerActor(mScene, cameraActor);

			player->SetPosition(glm::vec3(0, 0, 4));
			
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
			mTime += deltaTime;
			mFrames += 1;

			if (mTime >= 2.0f)
			{
				LOG_INFO("FPS: %f", mFrames / mTime);
				mFrames = 0;
				mTime = 0;
			}
		}

	private:
		float mTime = 0;
		int mFrames = 0;
	};
}