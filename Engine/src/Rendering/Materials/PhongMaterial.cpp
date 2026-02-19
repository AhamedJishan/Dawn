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
		// Renderer would bind Shader

		if (mDiffuse)
		{
			mDiffuse->Bind(0);
			mShader->SetInt("u_DiffuseTexture", 0);
			mShader->SetBool("u_HasDiffuseMap", true);
		}
		else
			mShader->SetBool("u_HasDiffuseMap", false);

		if (mSpecular)
		{
			mSpecular->Bind(1);
			mShader->SetInt("u_SpecularTexture", 1);
			mShader->SetBool("u_HasSpecularMap", true);
		}
		else
			mShader->SetBool("u_HasSpecularMap", false);

		if (mNormal)
		{
			mNormal->Bind(2);
			mShader->SetInt("u_NormalTexture", 2);
			mShader->SetBool("u_HasNormalMap", true);
		}
		else
			mShader->SetBool("u_HasNormalMap", false);

		mShader->SetVec3("u_DiffuseColor", mDiffuseColor);
		mShader->SetVec3("u_SpecularColor", mSpecularColor);
		mShader->SetFloat("u_Shininess", mShininess);
	}
}