#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>


class Chest {
public:
    glm::vec3 position;
    glm::vec2 size;

    Chest(float x,float y);
};
