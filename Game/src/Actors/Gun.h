#pragma once

#include "Core/Actor.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class Gun : public Actor
	{
	public:
		Gun(Scene* scene);

		void Update(float deltaTime) override;

		void Fire();

	private:
		const float mFireCooldown = 0.25f;
		float mTimeSinceLastFire = 0.0f;
	};
}