#include "spriteRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(Shader& shader)
    : shader(shader) {
    initRenderData();
    initWhiteTexture();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void SpriteRenderer::Draw(SpriteTexture& texture,
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    const glm::mat4& view,
    const glm::vec2& UVOffset,
    const glm::vec2& UVSize,
	const glm::vec4& color
) {
    shader.use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader.setMat4("uModel", model);
    shader.setMat4("uView", view);
	shader.setVec4("uColor", color);
    shader.setVec2("uUVOffset", UVOffset);
    shader.setVec2("uUVSize", UVSize);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawColor(
    const glm::vec2& position,
    const glm::vec2& size,
    const glm::vec4& color,
    const glm::mat4& view,
    float rotation
) {
    Draw(
        whiteTexture,
        position,
        size,
        rotation,
        view,
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        color
    );
}

void SpriteRenderer::initRenderData() {
    // pos (x,y)    uv (u,v)
    float vertices[] = {
        -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        0.5f, -0.5f,  1.0f, 0.0f,  // bottom-right
        0.5f,  0.5f,  1.0f, 1.0f,  // top-right
        -0.5f,  0.5f,  0.0f, 1.0f  // top-left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
        indices, GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );

    // uv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );

    glBindVertexArray(0);
}

void SpriteRenderer::initWhiteTexture() {
    unsigned char whitePixel[4] = { 255, 255, 255, 255 };
    whiteTexture.generate(1, 1, whitePixel);
}