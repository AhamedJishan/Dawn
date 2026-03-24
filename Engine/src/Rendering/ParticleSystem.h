#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	// Generic struct for over lifetime data types
	template<typename T>
	struct OverLifetime
	{
		struct Key
		{
			T value;
			float t;
		};

		// should be provided sorted
		std::vector<Key> keys;

		T Sample(float t)
		{
			if (keys.size() == 0) return T{};
			if (t <= keys.front().t) return keys.front().value;
			if (t >= keys.back().t) return keys.back().value;

			for (int i = 0; i < keys.size() - 1; i++)
			{
				if (t >= keys[i].t)
				{
					float localT = (t - keys[i].t) / (keys[i + 1].t - keys[i].t);
					return glm::mix(keys[i].value, keys[i + 1].value, localT);
				}
			}
			return keys.back().value;
		}
	};

	struct ParticleSystemDesc
	{
		bool isLooping = false;

		float duration = 1.0f;
		float particleLifetime = 1.0f;
		float emissionRate = 1.0f;
		float speed = 1.0f;
		
		unsigned int initialBurst = 0;
		unsigned int maxParticleCount = 50;
		
		glm::vec3 directionMin = glm::vec3(0);
		glm::vec3 directionMax = glm::vec3(0);

		OverLifetime<glm::vec3> scaleOverLifeTime;
		OverLifetime<glm::vec4> colorOverLifeTime;
	};

	struct ParticlePool
	{
		unsigned int particleCount = 0;

		std::vector<float> tValues;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> velocities;
	};

	// Forward declarations
	class Scene;
	class Shader;

	class ParticleSystem
	{
	public:
		ParticleSystem(Scene* scene, ParticleSystemDesc particleSystemDesc, glm::vec3 position);
		~ParticleSystem();

		void Update(float deltaTime);
		void Render(Shader* shader);

		const ParticlePool* GetParticlePool() const { return mParticlePool; }

		glm::vec3 GetPosition() const { return mPosition; }
		void SetPosition(glm::vec3 position) { mPosition = position; }

	private:
		void InitCubeVAO();
		void DestroyCubeVAO();
		void SpawnParticles(unsigned int count);

	private:
		Scene* mScene = nullptr;

		float mSystemTime = 0.0f;
		float mTimeSinceLastSpawn = 0.0f;

		bool mIsStopped = false;
		glm::vec3 mPosition = glm::vec3(0);

		ParticleSystemDesc mParticleSystemDesc;
		ParticlePool* mParticlePool = nullptr;

		unsigned int mCubeVAO = 0, mCubeVBO = 0, mCubeEBO = 0;
		unsigned int mParticleTVBO = 0, mParticlePositionVBO = 0;
	};
}