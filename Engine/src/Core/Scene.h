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
	class Application;
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
	private:
		friend class Application;
		void UpdateActors(float deltaTime);

		friend class Actor;
		void AddActor(Actor* actor);
		void RemoveActor(Actor* actor);

		friend class SphereCollider;
		void AddSphereCollider(SphereCollider* collider);
		void RemoveSphereCollider(SphereCollider* collider);

	public:
		Scene();
		virtual ~Scene();

		// Scenes can implement this to populate the Scene with Actors
		virtual void Init() {}
		virtual void Update(float deltaTime) {}
		virtual void ImGuiRender() {}


		bool ContainsActor(Actor* actor);

		void SetPaused(bool value) { mIsPaused = value; }
		bool IsPaused() const { return mIsPaused; }

		void SetActiveCamera(Camera* camera) { mActiveCamera = camera; }
		Camera* GetActiveCamera() const { return mActiveCamera; }

		// Physics Utilities
		const std::vector<SphereCollider*>& GetColliderList() const { return mColliders; }
		bool RayCast(const Physics::Ray& ray, float maxDistance, RaycastHit& outHitInfo);

	private:
		bool mUpdatingActors = false;
		bool mIsPaused = false;

		Camera* mActiveCamera = nullptr;

		std::vector<Actor*> mActors;
		std::vector<Actor*> mPendingActors;

		std::vector<SphereCollider*> mColliders;
	};
}