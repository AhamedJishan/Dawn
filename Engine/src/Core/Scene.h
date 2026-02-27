#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

namespace Dawn
{
	// Forward Declarations;
	class Actor;
	class Camera;
	class SphereCollider;
	namespace Physics
	{
		struct Ray;
		struct Sphere;
	}

	struct RaycastHit
	{
		Actor* actor;
		glm::vec3 position;
	};

	class Scene
	{
	public:
		Scene(const std::string& filePath);
		~Scene();

		void Update(float deltaTime);

		void AddActor(Actor* actor);
		void RemoveActor(Actor* actor);

		void AddSphereCollider(SphereCollider* collider);
		void RemoveSphereCollider(SphereCollider* collider);

		void SetActiveCamera(Camera* camera) { mActiveCamera = camera; }
		Camera* GetActiveCamera() const { return mActiveCamera; }

		// Physics Utilities
		bool RayCast(const Physics::Ray& ray, float maxDistance, RaycastHit& outHitInfo);

	private:
		std::string mFilePath;
		bool mUpdatingActors = false;

		Camera* mActiveCamera = nullptr;

		std::vector<Actor*> mActors;
		std::vector<Actor*> mPendingActors;

		std::vector<SphereCollider*> mColliders;
	};
}