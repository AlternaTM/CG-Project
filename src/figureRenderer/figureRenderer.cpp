#include "figureRenderer.h"

FigRenderer::FigRenderer(Shader& shader):shader(shader) {
	
}

FigRenderer::~FigRenderer() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
}

bool FigRenderer::init() {
    const float vertices[] = {
        0.0f, 0.0f,   
        1.0f, 0.0f,   
        1.0f, 1.0f,   
        0.0f, 1.0f    
    };
    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // location 0 -> vec2 aPos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
     
    return true;
}


void FigRenderer::drawRect(const glm::vec2& pos, const glm::vec2& size, const glm::mat4& view, const glm::vec4& color) {
    shader.use();
    shader.setVec2("uPosition", pos);
    shader.setVec2("uSize", size);
    shader.setMat4("uView", view);
    shader.setVec4("uColor", color);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FigRenderer::drawRect(const glm::vec2& pos, const glm::vec2& size,
    float angle, const glm::mat4& view, const glm::vec4& color) {
    shader.use();
    shader.setVec2("uPosition", pos);
    shader.setVec2("uSize", size);
    shader.setFloat("uAngle", angle); 
    shader.setMat4("uView", view);
    shader.setVec4("uColor", color);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}