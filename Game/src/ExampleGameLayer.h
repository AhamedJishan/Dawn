#pragma once

#include "Utils/Log.h"
#include "Core/Scene.h"
#include "Core/GameLayer.h"

#include "Core/Components/Camera.h"
#include "ExampleActor.h"
#include "TestCameraActor.h"

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

			ExampleActor* ea = new ExampleActor(GetScene());
			TestCameraActor* tca = new TestCameraActor(GetScene());

			tca->SetPosition(glm::vec3(0, 2, 2.5f));
			
			Camera* cam = tca->GetComponent<Camera>();
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

			if (mTime >= 1.0f)
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