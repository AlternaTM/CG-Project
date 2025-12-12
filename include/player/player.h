#pragma once

#include <glm/glm.hpp>
#include <shader/shader.h>

class Player {
public:
    Player();
    ~Player();

    void draw(const Shader& shader);

    glm::vec3 position;
    glm::vec3 mousePosition;
    // glm::vec3 camPosition;
    float speed;

private:
    unsigned int VAO, VBO, EBO;
	unsigned int lineVAO, lineVBO;
    void setupQuad();
    void setupLine();
};
