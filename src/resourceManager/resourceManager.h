#pragma once

#include <map>
#include <string>
#include "../spriteTexture/spriteTexture.h"

class ResourceManager {
public:
    static SpriteTexture& LoadTexture(const std::string& name,
        const char* file);

    static SpriteTexture& GetTexture(const std::string& name);
    static void Clear();

private:
    static std::map<std::string, SpriteTexture> Textures;
};