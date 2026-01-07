#include "resourceManager.h"
#include "../stb/stb_image.h"

#include <iostream>

std::map<std::string, Texture> ResourceManager::Textures;

Texture& ResourceManager::LoadTexture(const std::string& name,
    const char* file) {
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(file, &w, &h, &ch, 4);
    if (!data) {
        std::cerr << "FAILED TO LOAD TEXTURE: " << file << std::endl;
    }

    Texture tex;
    tex.generate(w, h, data);

    stbi_image_free(data);

    Textures[name] = tex;
    return Textures[name];
}

Texture& ResourceManager::GetTexture(const std::string& name) {
    return Textures.at(name);
}

void ResourceManager::Clear() {
    for (auto& it : Textures)
        glDeleteTextures(1, &it.second.ID);
    Textures.clear();
}
