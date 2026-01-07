#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Player {
public:
    glm::vec3 position;
    glm::vec3 mousePosition;
    float speed;

    Player();

    void update(float dt, GLFWwindow* window);
};
