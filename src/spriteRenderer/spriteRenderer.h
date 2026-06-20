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
        const glm::mat4& view,
        const glm::vec2& UVOffset = { 0.0f,0.0f },
        const glm::vec2& UVSize = { 1.0f,1.0f },
        const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    void DrawColor(
        const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec4& color,
        const glm::mat4& view = glm::mat4(1.0f),
        float rotation = 0.0f
    );

private:
    Shader& shader;
    unsigned int VAO, VBO, EBO;
	SpriteTexture whiteTexture;

    void initRenderData();
    void initWhiteTexture();
};
