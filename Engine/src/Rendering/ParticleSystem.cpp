#include "ParticleSystem.h"

#include "Utils/Random.h"
#include <glad/glad.h>
#include "Rendering/Shader.h"
#include "Core/Scene.h"

namespace Dawn
{
	ParticleSystem::ParticleSystem(Scene* scene, ParticleSystemDesc particleSystemDesc, glm::vec3 position)
		:mScene(scene)
		,mParticleSystemDesc(particleSystemDesc)
		,mParticlePool(new ParticlePool())
		,mPosition(position)
	{
		mScene->AddParticleSystem(this);

		InitCubeVAO();
		SpawnParticles(mParticleSystemDesc.initialBurst);
	}

	ParticleSystem::~ParticleSystem()
	{
		mScene->RemoveParticleSystem(this);
		DestroyCubeVAO();
	}
	
	void ParticleSystem::Update(float deltaTime)
	{
		mSystemTime += deltaTime;

		if (!mParticleSystemDesc.isLooping && mSystemTime >= mParticleSystemDesc.duration + mParticleSystemDesc.particleLifetime)
		{
			mIsStopped = true;
			return;
		}

		bool emissionActive = mParticleSystemDesc.isLooping || mSystemTime < mParticleSystemDesc.duration;
		if (emissionActive)
		{
			mTimeSinceLastSpawn += deltaTime;
			unsigned int particlesToSpawn = 0;
			float spawnInterval = 1.0f / mParticleSystemDesc.emissionRate;
			while (mTimeSinceLastSpawn >= spawnInterval)
			{
				mTimeSinceLastSpawn -= spawnInterval;
				particlesToSpawn++;
			}
			SpawnParticles(particlesToSpawn);
		}

		for (int i = mParticlePool->particleCount - 1; i >= 0; i--)
		{
			mParticlePool->tValues[i] += deltaTime / mParticleSystemDesc.particleLifetime;
			mParticlePool->positions[i] += mParticlePool->velocities[i] * deltaTime;

			// check for dead particles
			if (mParticlePool->tValues[i] >= 1.0f)
			{
				// swap with last and pop
				int last = mParticlePool->particleCount - 1;
				mParticlePool->tValues[i] = mParticlePool->tValues[last];
				mParticlePool->positions[i] = mParticlePool->positions[last];
				mParticlePool->velocities[i] = mParticlePool->velocities[last];

				mParticlePool->tValues.pop_back();
				mParticlePool->positions.pop_back();
				mParticlePool->velocities.pop_back();
				mParticlePool->particleCount--;
			}
		}
	}

	void ParticleSystem::Render(Shader* shader)
	{
		if (mParticlePool->particleCount == 0)
			return;

		shader->SetVec3s("u_ScaleOverTime",
			mParticleSystemDesc.scaleOverTime.GetValues().data(),
			mParticleSystemDesc.scaleOverTime.GetKeyCount());

		// upload tValues
		glBindBuffer(GL_ARRAY_BUFFER, mParticleTVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mParticlePool->particleCount * sizeof(float), mParticlePool->tValues.data());

		// upload positions
		glBindBuffer(GL_ARRAY_BUFFER, mParticlePositionVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mParticlePool->particleCount * 3 * sizeof(float), mParticlePool->positions.data());

		glBindVertexArray(mCubeVAO);
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, mParticlePool->particleCount);
		glBindVertexArray(0);
	}

	void ParticleSystem::SpawnParticles(unsigned int count)
	{
		if (count == 0)
			return;

		unsigned int particleCount = mParticlePool->particleCount;
		if (particleCount > mParticleSystemDesc.maxParticleCount)
			return;

		if (particleCount + count > mParticleSystemDesc.maxParticleCount)
			count = mParticleSystemDesc.maxParticleCount - particleCount;


		mParticlePool->tValues.reserve(mParticlePool->tValues.size() + count);
		mParticlePool->positions.reserve(mParticlePool->positions.size() + count);
		mParticlePool->velocities.reserve(mParticlePool->velocities.size() + count);

		for (unsigned int i = 0; i < count; i++)
		{
			glm::vec3 velocity = Random::Vec3(mParticleSystemDesc.directionMin, mParticleSystemDesc.directionMax);
			if (glm::length(velocity) > 0.0001f)
				velocity = glm::normalize(velocity) * mParticleSystemDesc.speed;
			else
				velocity = glm::vec3(0);

			mParticlePool->velocities.push_back(velocity);
			mParticlePool->tValues.push_back(0.0f);
			mParticlePool->positions.push_back(mPosition);
		}

		mParticlePool->particleCount += count;
	}

	void ParticleSystem::InitCubeVAO()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f, // 0
			 0.5f, -0.5f, -0.5f, // 1
			 0.5f,  0.5f, -0.5f, // 2
			-0.5f,  0.5f, -0.5f, // 3
			-0.5f, -0.5f,  0.5f, // 4
			 0.5f, -0.5f,  0.5f, // 5
			 0.5f,  0.5f,  0.5f, // 6
			-0.5f,  0.5f,  0.5f  // 7
		};

		// 12 triangles (36 indices)
		unsigned int indices[] = {
			// Back face
			0, 1, 2,
			2, 3, 0,
			// Front face
			4, 5, 6,
			6, 7, 4,
			// Left face
			7, 3, 0,
			0, 4, 7,
			// Right face
			1, 2, 6,
			6, 5, 1,
			// Bottom face
			0, 1, 5,
			5, 4, 0,
			// Top face
			3, 2, 6,
			6, 7, 3
		};

		glGenVertexArrays(1, &mCubeVAO);
		glGenBuffers(1, &mCubeVBO);
		glGenBuffers(1, &mCubeEBO);
		glGenBuffers(1, &mParticleTVBO);
		glGenBuffers(1, &mParticlePositionVBO);

		glBindVertexArray(mCubeVAO);

		// EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCubeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, mCubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// Particle tValues
		glBindBuffer(GL_ARRAY_BUFFER, mParticleTVBO);
		glBufferData(GL_ARRAY_BUFFER, mParticleSystemDesc.maxParticleCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
		
		// Particle ppositions
		glBindBuffer(GL_ARRAY_BUFFER, mParticlePositionVBO);
		glBufferData(GL_ARRAY_BUFFER, mParticleSystemDesc.maxParticleCount * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}

	void ParticleSystem::DestroyCubeVAO()
	{
		glDeleteBuffers(1, &mCubeVBO);
		glDeleteBuffers(1, &mCubeEBO);
		glDeleteBuffers(1, &mParticleTVBO);
		glDeleteBuffers(1, &mParticlePositionVBO);
		glDeleteVertexArrays(1, &mCubeVAO);
	}
}