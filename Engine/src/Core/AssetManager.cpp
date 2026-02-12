#include "AssetManager.h"
#include "Utils/Log.h"

#include "Rendering/Image.h"
#include "Rendering/Texture.h"

namespace Dawn
{
    AssetManager::AssetManager()
    {
        // Nothing for now
    }

    AssetManager::~AssetManager()
    {
        for (auto& it : mTextures)
            delete it.second;
        mTextures.clear();
        LOG_INFO("Destroyed all Textures");
    }

    Texture* AssetManager::GetTexture(const std::string& filePath)
    {
        auto it = mTextures.find(filePath);
        if (it != mTextures.end())
            return it->second;

        Image image(filePath);
        if (!image.IsValid())
            return nullptr;

        Texture* texture = new Texture(&image);        
        if (!texture->IsValid())
        {
            delete texture;
            return nullptr;
        }

        LOG_INFO("Loaded Texture: %s", filePath.c_str());
        mTextures.emplace(filePath, texture);

        return texture;
    }
}