#pragma once

#include <glm/vec3.hpp>
#include "Rendering/Material.h"

namespace Dawn
{
	// Forward declaration
	class Texture;

	class PhongMaterial : public Material 
	{
	public:
		PhongMaterial();
		~PhongMaterial();

		void Apply() override;

		void SetDiffuse(Texture* texture) { mDiffuse = texture; }
		void SetSpecular(Texture* texture) { mSpecular = texture; }
		void SetNormal(Texture* texture) { mNormal = texture; }

		void SetDiffuseColor(const glm::vec3& color) { mDiffuseColor = color; }
		void SetSpecularColor(const glm::vec3& color) { mSpecularColor = color; }

		void SetShininess(float shininess) { mShininess = shininess; }

	private:
		Texture* mDiffuse = nullptr;
		Texture* mSpecular= nullptr;
		Texture* mNormal= nullptr;

		glm::vec3 mDiffuseColor = glm::vec3(1.0f);
		glm::vec3 mSpecularColor = glm::vec3(1.0f);

		float mShininess = 32.0f;
	};
}