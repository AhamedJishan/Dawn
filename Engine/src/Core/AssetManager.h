#pragma once

#include <unordered_map>
#include <string>

namespace Dawn
{
	// Forward declarations
	class Texture;
	class Shader;

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		Texture* GetTexture(const std::string& filePath);
		Shader* GetShader(const std::string& shaderName);

	private:
		Shader* CreateShaderByName(const std::string& name);

	private:
		std::unordered_map<std::string, Texture*> mTextures;
		std::unordered_map<std::string, Shader*> mShaders;
	};
}