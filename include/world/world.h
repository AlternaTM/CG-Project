#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <shader/shader.h>
#include <string>

class World {
public:
    World(const std::string& bgTexturePath, float worldWidth, float worldHeight);
    ~World();

    void draw(Shader& shader);

private:
    void setupQuad();
    unsigned int backgroundTexture;
    unsigned int bgVAO, bgVBO, bgEBO;
    float width, height;
};