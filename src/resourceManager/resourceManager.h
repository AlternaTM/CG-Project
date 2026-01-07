#pragma once

#include <map>
#include <string>
#include "../texture/texture.h"

class ResourceManager {
public:
    static Texture& LoadTexture(const std::string& name,
        const char* file);

    static Texture& GetTexture(const std::string& name);
    static void Clear();

private:
    static std::map<std::string, Texture> Textures;
};