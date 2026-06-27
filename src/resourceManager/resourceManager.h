#pragma once

#include <map>
#include <string>
#include "../spriteTexture/spriteTexture.h"
#include "../modelRenderer/modelRenderer.h"


class ResourceManager {
public:
    static SpriteTexture& LoadTexture(const std::string& name,
        const char* file);

    static SpriteTexture& GetTexture(const std::string& name);



    static ModelRenderer& LoadModel(const std::string& name,const std::string& path, Shader* shader);
    static ModelRenderer& GetModel(const std::string& name);
    static void addModel(const std::string& name, ModelRenderer* modelRenderer);
    static void clearModels();


    static Shader& LoadShader(
        const std::string& name,
        const std::string& vertPath,
        const std::string& fragPath
    );
    static Shader& GetShader(const std::string& name);
    static void clearShaders();

    static void Clear();

private:
    static std::map<std::string, SpriteTexture> Textures;
    static std::map<std::string, ModelRenderer*> models;
    static std::map<std::string, Shader*> shaders;
};