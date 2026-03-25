#pragma once

#include <glm/vec3.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include "../chest/chest.h"
#include "../collision/collision.h"
#include <unordered_map>
#include "player.h"

class Player;
class Chest;

class PlayerInput {
private: 
	static std::unordered_map<int, bool> wasPressed;
public:
	static void register_input(int l);
	static bool isPressed(GLFWwindow* window, int l);
	static bool isKeyJustPressed(GLFWwindow* window, int l);
	static void update_input(GLFWwindow* window);
	static bool move(GLFWwindow* window, Player& pl, float speed, float dt);
	static void interact(GLFWwindow* window, Player& pl);
	static void updateMouse(GLFWwindow* window, glm::vec2 cameraPos, glm::vec3 playerPos, glm::vec3& aimPos, float& aimRotation);
};
