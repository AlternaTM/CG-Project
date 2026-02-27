#pragma once

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include "../chest/chest.h"
#include "../collision/collision.h"
#include <unordered_map>

class PlayerInput {

public:
	static bool move(GLFWwindow* window, Player& pl, float speed, float dt);
	static void interact(GLFWwindow* window, Player& pl,std::vector<Chest>& chests);
	static void updateMouse(GLFWwindow* window, glm::vec2 cameraPos, glm::vec3 playerPos, glm::vec3& aimPos, float& aimRotation);
};
