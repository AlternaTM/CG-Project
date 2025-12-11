#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <player/playerInput.h>

PlayerInput::PlayerInput() {

}

PlayerInput::~PlayerInput() {

}

void PlayerInput::move(GLFWwindow* window, glm::vec3& pos, float speed, float dt) {
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		// Move
		pos.y += speed * dt;
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		// Move
		pos.y -= speed * dt;
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		// Move
		pos.x -= speed * dt;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		// Move
		pos.x += speed * dt;
	}

	// Da modificare con i limiti della mappa
	pos.x = glm::clamp(pos.x, -16.0f + 0.5f,  16.0f - 0.5f);
	pos.y = glm::clamp(pos.y, -9.0f + 0.5f,  9.0f - 0.5f);
}