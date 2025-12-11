#pragma once

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

class PlayerInput {
private:
public:
	PlayerInput();
	~PlayerInput();

	void move(GLFWwindow* window, glm::vec3& pos, float speed, float dt);
};
