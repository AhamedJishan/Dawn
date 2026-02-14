#include "PhongMaterial.h"

#include "Core/Assets.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

namespace Dawn
{
	PhongMaterial::PhongMaterial()
	{
		mShader = Assets::GetShader("phong");
	}

	PhongMaterial::~PhongMaterial()
	{
	}
	
	void PhongMaterial::Apply()
	{
		if (mAlbedo)
		{
			mAlbedo->Bind(0);
			mShader->SetInt("u_Albedo", 0);
			mShader->SetInt("u_HasAlbedo", 1);
		}
		else
			mShader->SetInt("u_HasAlbedo", 0);
	}
}