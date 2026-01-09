#pragma once

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include "../chest/chest.h"
#include "../collision/collision.h"

class PlayerInput {
private:
public:
	static void move(GLFWwindow* window, glm::vec3& pos, float speed, float dt);
	static void updateMouse(GLFWwindow* window, glm::vec3& mousePos);
	static void interact(GLFWwindow* window, Player& pl,std::vector<Chest>& chests);
};
