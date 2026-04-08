#pragma once

#include "Core/Actor.h"

namespace Dawn
{
	// Forward declarations
	class Gun;
	class Player;

	class UpgradeManager : public Actor
	{
	public:
		UpgradeManager(Scene* scene, Player* player, Gun* gun);

		void UpgradeSpread();
		void UpgradeDamage();
		void UpgradeDash();

	private:
		Player* mPlayer = nullptr;
		Gun* mGun = nullptr;
	};
}