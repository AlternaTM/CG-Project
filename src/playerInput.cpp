#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <player/playerInput.h>

#include <iostream>

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

void PlayerInput::updateMouseOrientation(GLFWwindow* window, glm::vec3& mousePos) {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//std::cout << "Mouse Position (pixels): " << mouseX << ", " << mouseY << std::endl;

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// !!!
	/*
	float aspect = (float)width / (float)height;
	float screenHeight = 9.0f;
	float screenWidth = screenHeight * aspect;
	*/

	glm::vec3 win(mouseX, mouseY, 0.0f);
	glm::vec4 viewport(0.0f, 0.0f, (float)width, (float)height);

	glm::vec3 realPos = glm::unProject(win, glm::mat4(1.0f), glm::mat4(1.0f), viewport);

	mousePos.x = realPos.x;
	mousePos.y = -realPos.y;



	//std::cout << "Mouse Position (world): " << mousePos.x << ", " << mousePos.y << std::endl;

	/*
	mousePos.x = (float(mouseX) / float(width)) * screenWidth - (screenWidth / 2.0f);
	mousePos.y = -((float(mouseY) / float(height)) * screenHeight - (screenHeight / 2.0f));
	*/
}