#pragma once

#include "Core/Component.h"

namespace Dawn
{
	// Forward declarations
	class Actor;

	class Damageable : public Component
	{
	public:
		Damageable(Actor* owner, float maxHealth = 100.0f)
			:Component(owner)
			, mHealth(maxHealth)
		{
		}

		void TakeDamage(float dmg)
		{
			mHealth -= dmg;

			if (mHealth <= 0.0f)
				mIsDead = true;
		}

		float GetHealth() const { return mHealth; }
		bool IsDead() const { return mIsDead; }

	private:
		float mHealth = 100.0f;
		bool mIsDead = false;
	};
}