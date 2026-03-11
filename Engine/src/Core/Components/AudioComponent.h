#pragma once

#include "Core/Component.h"

#include <string>
#include <vector>

namespace Dawn
{
	// Forward declarations
	class Actor;
	class SoundEvent;

	class AudioComponent : public Component
	{
	public:
		AudioComponent(Actor* owner, int updateOrder = 200);
		~AudioComponent();

		void Update(float deltaTime) override;

		SoundEvent PlayEvent(const std::string& name);
		void StopAllEvents();

	private:
		std::vector<SoundEvent> mEvents2D;
		std::vector<SoundEvent> mEvents3D;
	};
}