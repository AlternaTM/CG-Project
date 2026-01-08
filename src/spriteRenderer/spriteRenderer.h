#pragma once

#include <glm/glm.hpp>
#include "../shader/shader.h"
#include "../spriteTexture/spriteTexture.h"

class SpriteRenderer {
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

    void Draw(SpriteTexture& texture,
        const glm::vec2& position,
        const glm::vec2& size,
        float rotation,
        const glm::mat4& view);

private:
    Shader& shader;
    unsigned int VAO, VBO, EBO;

    void initRenderData();
};
