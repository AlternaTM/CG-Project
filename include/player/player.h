#pragma once

#include <glm/glm.hpp>
#include <shader/shader.h>

class Player {
public:
    Player();
    ~Player();

    void draw(const Shader& shader);

    glm::vec3 position;
    float speed;

private:
    unsigned int VAO, VBO, EBO;
    void setupQuad();
};
