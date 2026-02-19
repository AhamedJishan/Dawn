#pragma once

#include <glm/vec3.hpp>
#include "Rendering/Material.h"

namespace Dawn
{
	// Forward declaration
	class Texture;
	class RawMaterial;

	class PhongMaterial : public Material 
	{
	public:
		PhongMaterial();
		~PhongMaterial();

		static PhongMaterial* CreateFromRaw(const RawMaterial* rawMaterial);

		void Apply() override;

		void SetDiffuseMap(Texture* texture) { mDiffuseMap = texture; }
		void SetSpecularMap(Texture* texture) { mSpecularMap = texture; }
		void SetNormalMap(Texture* texture) { mNormalMap = texture; }

		void SetDiffuseColor(const glm::vec3& color) { mDiffuseColor = color; }
		void SetSpecularColor(const glm::vec3& color) { mSpecularColor = color; }

		void SetShininess(float shininess) { mShininess = shininess; }

	private:
		Texture* mDiffuseMap = nullptr;
		Texture* mSpecularMap = nullptr;
		Texture* mNormalMap = nullptr;

		glm::vec3 mDiffuseColor = glm::vec3(1.0f);
		glm::vec3 mSpecularColor = glm::vec3(1.0f);

		float mShininess = 32.0f;
	};
}