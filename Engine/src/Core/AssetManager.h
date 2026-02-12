#pragma once

#include <unordered_map>
#include <string>

namespace Dawn
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		class Texture* GetTexture(const std::string& filePath);

	private:
		std::unordered_map<std::string, class Texture*> mTextures;
	};
}