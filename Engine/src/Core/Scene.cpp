#include "Scene.h"

#include <algorithm>
#include "Actor.h"
#include "Physics/Physics.h"
#include "Components/SphereCollider.h"

namespace Dawn
{
	Scene::Scene(const std::string& filePath)
		:mFilePath(filePath)
	{
		// TODO: Load the scene from the filepath. To be done when/if Serialization is implemented
	}

	// Relies on Actor::~Actor() to call Scene::RemoveActor()
	Scene::~Scene()
	{
		while (!mActors.empty())
			delete mActors.back();

		while (!mPendingActors.empty())
			delete mPendingActors.back();
	}
	
	void Scene::Update(float deltaTime)
	{
		// Update Actors
		mUpdatingActors = true;
		for (Actor* actor : mActors)
			actor->UpdateActor(deltaTime);
		mUpdatingActors = false;

		// Move pending actors to mActors
		for (Actor* actor : mPendingActors)
			mActors.emplace_back(actor);
		mPendingActors.clear();

		// Gather the actors to be deleted
		std::vector<Actor*> deadActors;
		for (Actor* actor : mActors)
			if (actor->GetState() == Actor::State::Dead)
				deadActors.emplace_back(actor);

		// Delete dead actors
		for (Actor* actor : deadActors)
			delete actor;					// Actor::~Actor() calls Scene::RemoveActor() to remove itself from mActors
		deadActors.clear();
	}
	
	void Scene::AddActor(Actor* actor)
	{
		if (mUpdatingActors)
			mPendingActors.emplace_back(actor);
		else
			mActors.emplace_back(actor);
	}
	
	// Called by Actor::~Actor()
	// Removes the actor from both active and pending lists.
	void Scene::RemoveActor(Actor* actor)
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end())
			mActors.erase(it);

		it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		if (it != mPendingActors.end())
			mPendingActors.erase(it);

	}

	void Scene::AddSphereCollider(SphereCollider* collider)
	{
		mColliders.emplace_back(collider);
	}
	// Called by SphereCollider::~SphereCollider()
	// Removes the SphereCollider from the list.
	void Scene::RemoveSphereCollider(SphereCollider* collider)
	{
		auto it = std::find(mColliders.begin(), mColliders.end(), collider);
		if (it != mColliders.end())
			mColliders.erase(it);
	}

	bool Scene::RayCast(const Physics::Ray& ray, float maxDistance, RaycastHit& outHitInfo)
	{
		float closestT = maxDistance;
		bool hit = false;
		outHitInfo.actor = nullptr;

		for (SphereCollider* sphereCollider : mColliders)
		{
			Physics::Sphere sphere = sphereCollider->GetWorldSphere();

			float t = 0.0f;
			if (Physics::Intersects(ray, sphere, t) && t < closestT)
			{
				closestT =  t;
				hit = true;
				outHitInfo.actor = sphereCollider->GetOwner();
				outHitInfo.position = ray.origin + ray.direction * t;
			}
		}

		return hit;
	}
}