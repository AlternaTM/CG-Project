#include "resourceManager.h"
#include "../stb/stb_image.h"


#include <iostream>

std::map<std::string, SpriteTexture> ResourceManager::Textures;


SpriteTexture& ResourceManager::LoadTexture(const std::string& name,
    const char* file) {
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(file, &w, &h, &ch, 4);
    if (!data) {
        std::cerr << "FAILED TO LOAD TEXTURE: " << file << std::endl;
    }

    SpriteTexture tex;
    tex.generate(w, h, data);
    
    stbi_image_free(data);

    Textures[name] = tex;
    return Textures[name];
}

SpriteTexture& ResourceManager::GetTexture(const std::string& name) {
    return Textures.at(name);
}



//============CLEAR


void ResourceManager::Clear() {
    for (auto& it : Textures)
        glDeleteTextures(1, &it.second.ID);
    Textures.clear();
}


//============Models

std::map<std::string, ModelRenderer*> ResourceManager::models;


ModelRenderer& ResourceManager::LoadModel(
    const std::string& name,
    const std::string& path,
    const std::string& vertShader,
    const std::string& fragShader)
{
    if (models.find(name) != models.end())
        return *models[name];

    ModelRenderer* mr = new ModelRenderer(path, vertShader, fragShader);
    models[name] = mr;
    return *mr;
}

ModelRenderer& ResourceManager::GetModel(const std::string& path)
{
    if (models.find(path) == models.end())
        throw std::runtime_error("ResourceManager: modello non trovato: " + path);

    return *models[path];
}


void ResourceManager::addModel(const std::string& name, ModelRenderer* modelRenderer)
{
    if (models.find(name) != models.end())
        delete models[name];  

    models[name] = modelRenderer;
}



void ResourceManager::clearModels()
{
    for (auto& pair : models)
        delete pair.second;  

    models.clear();
}