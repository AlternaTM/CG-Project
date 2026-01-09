#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "camera/camera.h"

class Player {
public:
    glm::vec3 position;
    float speed;
    Camera& camera;
    glm::vec3 aimPosition;
    float aimRotation;

    Player(Camera& camera);

    void update(float dt, GLFWwindow* window);
};
