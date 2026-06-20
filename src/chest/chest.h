#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "camera/camera.h"
#include "spriteRenderer/spriteRenderer.h"
#include "modelRenderer/modelRenderer.h"
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
    float angle = 0.0f;
    bool finished = false;

    ChestManager(int n);
    bool interact(GLFWwindow* window, Player& pl);
    void render(SpriteRenderer& renderer, Camera& camera);

    void render_chest(Camera3D& camera3D, const glm::mat4& projection3D, ModelRenderer& chest, ModelRenderer& chest_lid, float dt);

};