#pragma once

#include "Core/Layer.h"
#include "Utils/Log.h"

namespace Dawn
{
	class ExampleLayer : public Layer
	{
	public:
		void OnAttach() override
		{
			LOG_WARN("Example Layer attached!");
		}

		void OnDetach() override
		{
			LOG_WARN("Example Layer detached!");
		}

		void OnUpdate(float deltaTime) override
		{
			mTime += deltaTime;
			mFrames += 1;

			if (mTime >= 1.0f)
			{
				LOG_INFO("FPS: %f", mFrames / mTime);
				mFrames = 0;
				mTime = 0;
			}
		}

	private:
		float mTime = 0;
		int mFrames = 0;
	};
}