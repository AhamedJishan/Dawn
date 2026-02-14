#pragma once

#include "Material.h"

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

		void SetAlbedo(Texture* texture) { mAlbedo = texture; }

	private:
		Texture* mAlbedo = nullptr;
	};
}