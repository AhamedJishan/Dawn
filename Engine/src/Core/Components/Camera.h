#pragma once

#include "Core/Component.h"
#include <glm/mat4x4.hpp>

namespace Dawn
{
	// Forward declaration
	class Actor;

	class Camera : public Component
	{
	public:
		Camera(Actor* owner, unsigned int updateOrder = 100);

		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;
		
		// FOV in degrees
		void SetFOV(float value) { mFOV = value; }
		void SetNearPlane(float value) { mNear = value; }
		void SetFarPlane(float value) { mFar = value; }

		// FOV in degrees
		float GetFOV() const { return mFOV; }
		float GetNearPlane() const { return mNear; }
		float GetFarPlane() const { return mFar; }

	protected:
		// in degrees
		float mFOV = 60.0f;
		float mNear = 0.1f;
		float mFar = 100.0f;
	};
}