#pragma once

#include <string>
#include <glm/vec3.hpp>

namespace Dawn
{
	// CPU-side material specification loaded from a model file (no GPU resources),
	// used to construct runtime Material instances.
	class RawMaterial
	{
	public:
		// Setters
		void SetDiffuseTexturePath(const std::string& diffuseTexturePath) { mDiffuseTexturePath = diffuseTexturePath; }
		void SetSpecularTexturePath(const std::string& specularTexturePath) { mSpecularTexturePath = specularTexturePath; }
		void SetNormalTexturePath(const std::string& normalTexturePath) { mNormalTexturePath = normalTexturePath; }

		void SetDiffuseColor(const glm::vec3& diffuseColor) { mDiffuseColor = diffuseColor; }
		void SetSpecularColor(const glm::vec3& specularColor) { mSpecularColor = specularColor; }

		void SetShininess(float shininess) { mShininess = shininess; }

		// Getters
		const std::string& GetDiffuseTexturePath() const { return mDiffuseTexturePath; }
		const std::string& GetSpecularTexturePath() const { return mSpecularTexturePath; }
		const std::string& GetNormalTexturePath() const { return mNormalTexturePath; }

		const glm::vec3& GetDiffuseColor() const { return mDiffuseColor ; }
		const glm::vec3& GetSpecularColor() const { return mSpecularColor ; }

		float GetShininess() const { return mShininess; }

	private:
		std::string mDiffuseTexturePath;
		std::string mSpecularTexturePath;
		std::string mNormalTexturePath;

		glm::vec3 mDiffuseColor = glm::vec3(1);
		glm::vec3 mSpecularColor = glm::vec3(1);

		float mShininess = 32.0f;
	};
}