#pragma once

#include "Core/Actor.h"

#include "Components/FPSCamera.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class FPSCameraActor : public Actor
	{
	public:
		FPSCameraActor(Scene* scene)
			:Actor(scene)
		{
			mCamera = new FPSCamera(this);
		}

		float GetYaw() const { return mCamera->GetYaw(); }
		float GetPitch() const { return mCamera->GetPitch(); }
	private:
		FPSCamera* mCamera = nullptr;
	};
}