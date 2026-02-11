#pragma once

#include "Layer.h"

namespace Dawn
{
	class GameLayer : public Layer
	{
	public:
		GameLayer();
		virtual ~GameLayer();

		// To be implemented by derived class
		virtual void OnGameUpdate(float deltaTime) {}

		void OnUpdate(float deltaTime) override;

		// Transfers ownership of the given Scene to this GameLayer.
		// Deletes any previously loaded Scene.
		// TODO: via serialization, load a scene object
		void LoadScene(class Scene* scene);
		class Scene* GetScene() const { return mScene; }

	protected:
		class Scene* mScene = nullptr;
	};
}