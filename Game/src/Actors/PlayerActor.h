#pragma once

#include "Core/Actor.h"
#include "Utils/Log.h"

#include "FPSCameraActor.h"
#include "Gun.h"

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

			mGun = new Gun(mScene);
		}

		void Update(float deltaTime) override
		{
			// sync camera pos
			mCamera->SetPosition(GetPosition() + mCameraOffset);

			float yaw = mCamera->GetYaw();
			SetRotation(glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)));

			// WASD movement
			float speed = mSpeed;
			if (Input::GetKey(Key::LeftShift)) speed = 2 * mSpeed;
			if (Input::GetKey(Key::W)) mPosition += speed * GetForward() * deltaTime;
			if (Input::GetKey(Key::S)) mPosition -= speed * GetForward() * deltaTime;
			if (Input::GetKey(Key::A)) mPosition -= speed * GetRight() * deltaTime;
			if (Input::GetKey(Key::D)) mPosition += speed * GetRight() * deltaTime;

			// sync Gun pos
			glm::vec3 gunPos = mCamera->GetPosition();
			gunPos += mCamera->GetUp() * mGunOffset.y;
			gunPos += mCamera->GetRight() * mGunOffset.x;
			gunPos += mCamera->GetForward() * mGunOffset.z;
			mGun->SetPosition(gunPos);
			// sync gun rotation
			mGun->SetRotation(mCamera->GetRotation());
		}

	private:
		float mSpeed = 3.0f;

		FPSCameraActor* mCamera = nullptr;
		glm::vec3 mCameraOffset = glm::vec3(0, 2, 0);

		Gun* mGun = nullptr;
		glm::vec3 mGunOffset = glm::vec3(0.15f, -.075f, 0.225f);
	};
}