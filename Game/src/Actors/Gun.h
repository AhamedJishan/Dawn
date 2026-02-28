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

		void Shoot();

	private:
	};
}