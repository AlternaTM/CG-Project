#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "camera/camera.h"
#include "spriteRenderer/spriteRenderer.h"
#include "collision/collision.h"
#include "player/player.h"



class Entity;

class Chest : public Entity{
public:
    Chest(float x,float y);
};


class ChestManager {
private:
    std::vector<Chest> chests;

public: 
    ChestManager(int n);
    bool interact(GLFWwindow* window, Player& pl);
    void render(SpriteRenderer& renderer, Camera& camera);
};