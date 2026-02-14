#include "AssetManager.h"
#include "Utils/Log.h"

#include "Rendering/Image.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"

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

        for (auto& it : mShaders)
            delete it.second;
        mShaders.clear();
        LOG_INFO("Destroyed all Shaders");
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

    Shader* AssetManager::GetShader(const std::string& shaderName)
    {
        auto it = mShaders.find(shaderName);
        if (it != mShaders.end())
            return it->second;

        Shader* shader = CreateShaderByName(shaderName);

        if (!shader)
        {
            LOG_ERROR("'%s' shader doesn't exist", shaderName.c_str());
            return nullptr;
        }
            
        mShaders[shaderName] = shader;
        return shader;
    }

    Shader* AssetManager::CreateShaderByName(const std::string& name)
    {
        Shader* shader = nullptr;

        if (name == "phong")
        {
            shader = new Shader("Assets/Shaders/phong.vert", "Assets/Shaders/phong.frag");
        }

        if (!shader || !shader->IsValid())
        {
            delete shader;
            return nullptr;
        }

        return shader;
    }
}