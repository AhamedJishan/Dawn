#pragma once

#include <string>
#include "Application.h"
#include "AssetManager.h"
#include "Rendering/Texture.h"

namespace Dawn
{
	// Facade for AssetManager
	class Asset
	{
	public:
		Texture* GetTexture(const std::string& filePath)
		{
			Application::Get()->GetAssetManager()->GetTexture(filePath);
		}
	};
}