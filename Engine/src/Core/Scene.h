#pragma once

#include <vector>
#include <string>

namespace Dawn
{
	// Forward Declarations;
	class Actor;
	class Camera;

	class Scene
	{
	public:
		Scene(const std::string& filePath);
		~Scene();

		void Update(float deltaTime);

		void AddActor(Actor* actor);
		void RemoveActor(Actor* actor);

		void SetActiveCamera(Camera* camera) { mActiveCamera = camera; }
		Camera* GetActiveCamera() const { return mActiveCamera; }

	private:
		std::string mFilePath;
		bool mUpdatingActors = false;

		Camera* mActiveCamera = nullptr;

		std::vector<Actor*> mActors;
		std::vector<Actor*> mPendingActors;
	};
}