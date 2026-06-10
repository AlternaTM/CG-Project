#include "figureRenderer.h"
#include <GLFW/glfw3.h>

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



void FigRenderer::drawRect(const glm::vec2& pos, const glm::vec2& size, float angle, const glm::mat4& view, const glm::vec4& color) {
    shader.use();
    shader.setVec2("uPosition", pos);
    shader.setVec2("uSize", size);
    shader.setFloat("uAngle", angle); 
    shader.setMat4("uView", view);
    shader.setVec4("uColor", color);
    shader.setFloat("uTime",(float) glfwGetTime());

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void FigRenderer::draw(
    const Mesh2D& mesh,
    const glm::mat4& transform,
    const glm::mat4& view,
    const glm::vec4& color
) {
    shader.use();
    shader.setMat4("uTransform", transform);
    shader.setMat4("uView", view);

    shader.setVec4("uColor", color);
    shader.setFloat("uTime", (float)glfwGetTime());


    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}


// ------------- MESH ----------------------

void Mesh2D::create(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices) {
    indexCount = indices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    
   

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex2D),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex2D),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); 
}


void Mesh2D::destroy() {
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (ebo)
        glDeleteBuffers(1, &ebo);
}