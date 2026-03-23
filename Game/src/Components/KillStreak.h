#pragma once

#include "Core/Component.h"
#include "Utils/Log.h"

namespace Dawn
{
	// Forward declarations
	class Actor;

	class KillStreak : public Component
	{
	public:
		KillStreak(Actor* owner)
			:Component(owner)
		{
		}

		unsigned int GetKillStreak() const { return mKillStreak; }

		void ShotMissed()
		{
			if (mKillStreak > 0)
				mKillStreak = 0;
		}

		void EnemyKilled()
		{
			mKillStreak++;
		}

	private:
		unsigned int mKillStreak = 0;
	};
}