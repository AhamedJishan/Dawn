#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	// Base class for all scene objects.
	// Owns transform state and a set of Components.
	class Actor
	{
	public:
		enum class State
		{
			Active,
			Paused,
			Dead
		};

		// For now, takes nothing as constructor.
		// TODO: Take scene or layer as paramter and automatically register itself to the corresponding scene.
		Actor();
		virtual ~Actor();

		// To be called by Scene
		void UpdateActor(float deltaTime);
		// To be implemented by custom Actor
		virtual void Update(float deltaTime) {}

		// Component managment
		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

		glm::mat4 GetWorldTransform() const;

		State GetState()		const { return mState; }
		glm::vec3 GetScale()	const { return mScale; }
		glm::vec3 GetPosition() const { return mPosition; }
		glm::quat GetRotation() const { return mRotation; }
		void SetState			(State state)				{ mState = state; }
		void SetScale			(const glm::vec3& scale)	{ mScale = scale; }
		void SetPosition		(const glm::vec3& position) { mPosition = position; }
		void SetRotation		(const glm::quat& rotation) { mRotation = rotation; }
		// Angle is in radians
		void Rotate(float angle, const glm::vec3& axisOfRotation);

	protected:
		State mState = State::Active;
		glm::vec3 mPosition = glm::vec3(0);
		glm::vec3 mScale = glm::vec3(1);
		glm::quat mRotation = glm::quat(1, 0, 0, 0);

		std::vector<class Component*> mComponents;
	};
}