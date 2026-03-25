#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "camera/camera.h"
#include "spriteRenderer/spriteRenderer.h"
#include "collision/collision.h"

class Player;
class PlayerInput;

class Chest {
public:
    glm::vec3 position;
    glm::vec2 size;

    Chest(float x,float y);
};


class ChestManager {
private:
    std::vector<Chest> chests;

public: 
    ChestManager(int n);
    void interact(GLFWwindow* window, Player& pl);
    void render(SpriteRenderer& renderer, Camera& camera);
};