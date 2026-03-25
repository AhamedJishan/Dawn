#pragma once
#include "Utils/Log.h"

#include <array>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Dawn
{
	// should remain in sync with particle.vert shader
	const unsigned int OVERTIME_MAX_KEYS = 5;

	// Generic struct for over time data types
	template<typename T>
	struct OverTime
	{
		void AddKey(float time, T value)
		{
			if (mKeyCount >= OVERTIME_MAX_KEYS)
			{
				LOG_WARN("OverLifetime struct can only have max 5 keys!");
				return;
			}

			mValues[mKeyCount] = value;
			mTimes[mKeyCount] = time;

			mKeyCount++;
		}

		int GetKeyCount() const { return mKeyCount; }
		const std::array<T, OVERTIME_MAX_KEYS>& GetValues() const { return mValues; }
		const std::array<float, OVERTIME_MAX_KEYS>& GetTimes() const { return mTimes; }

	private:
		// should be provided sorted
		std::array<T, OVERTIME_MAX_KEYS> mValues{};
		std::array<float, OVERTIME_MAX_KEYS> mTimes{};
		int mKeyCount = 0;
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

		OverTime<glm::vec3> scaleOverTime;
		OverTime<glm::vec4> colorOverTime;
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

		bool IsStopped() { return mIsStopped; }
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