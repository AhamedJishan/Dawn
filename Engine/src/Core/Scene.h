#pragma once

#include <vector>
#include <string>

namespace Dawn
{
	class Scene
	{
	public:
		Scene(const std::string& filePath);
		~Scene();

		void Update(float deltaTime);

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

	private:
		std::string mFilePath;
		bool mUpdatingActors = false;

		std::vector<class Actor*> mActors;
		std::vector<class Actor*> mPendingActors;
	};
}