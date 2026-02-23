#pragma once

#include "Core/Actor.h"
#include "Utils/Log.h"

#include "FPSCameraActor.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class PlayerActor : public Actor
	{
	public:
		PlayerActor(Scene* scene, FPSCameraActor* fpsCameraActor)
			:Actor(scene)
		{
			if (!fpsCameraActor)
			{
				LOG_WARN("Player Actor needs valid 'FPSCameraActor'");
				return;
			}

			mCamera = fpsCameraActor;
		}

		void Update(float deltaTime) override
		{
			// sync camera pos
			mCamera->SetPosition(GetPosition() + mCameraOffset);

			float yaw = mCamera->GetYaw();
			SetRotation(glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)));

			if (Input::GetKey(Key::W)) mPosition += mSpeed * GetForward() * deltaTime;
			if (Input::GetKey(Key::S)) mPosition -= mSpeed * GetForward() * deltaTime;
			if (Input::GetKey(Key::A)) mPosition -= mSpeed * GetRight() * deltaTime;
			if (Input::GetKey(Key::D)) mPosition += mSpeed * GetRight() * deltaTime;
		}

	private:
		FPSCameraActor* mCamera = nullptr;

		float mSpeed = 2.0f;
		glm::vec3 mCameraOffset = glm::vec3(0, 2, 0);
	};
}