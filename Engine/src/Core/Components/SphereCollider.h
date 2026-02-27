#pragma once

#include "Core/Component.h"

#include <glm/vec3.hpp>

namespace Dawn
{
	// Forward declarations
	struct Sphere;

	class SphereCollider : public Component
	{
	public:
		SphereCollider(class Actor* owner, unsigned int updateOrder = 100);

		Sphere GetWorldSphere() const;

		float GetRadius() const { return mRadius; }
		glm::vec3 GetOffset() const { return mOffset; }

		void SetRadius(float radius) { mRadius = radius; }
		void SetOffset(glm::vec3 offset) { mOffset = offset; }

	private:
		float mRadius = 1.0f;
		glm::vec3 mOffset = glm::vec3(0);
	};
}