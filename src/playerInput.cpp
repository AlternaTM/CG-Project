#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <playerInput/playerInput.h>

PlayerInput::PlayerInput() {

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

	pos.x = glm::clamp(pos.x, -1.0f + 0.2f,  1.0f - 0.2f);
	pos.y = glm::clamp(pos.y, -1.0f + 0.1f,  1.0f - 0.1f);
}
