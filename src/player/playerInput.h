#pragma once

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

class PlayerInput {
private:
public:
	static void move(GLFWwindow* window, glm::vec3& pos, float speed, float dt);
	static void updateMouse(GLFWwindow* window, glm::vec2 cameraPos, glm::vec3 playerPos, glm::vec3& aimPos, float& aimRotation);
};
