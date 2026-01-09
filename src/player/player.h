#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "camera/camera.h"

class Player {
public:
    glm::vec3 position;
    glm::vec3 mousePosition;
    float speed;
    float rotation;
    Camera& camera;

    Player(Camera& camera);

    void update(float dt, GLFWwindow* window);
};
