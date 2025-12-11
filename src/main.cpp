#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <shader/shader.h>
#include <player/playerInput.h>
#include <player/player.h>
#include <world/world.h>

//#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Toy Survivor", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//std::cerr << "CWD: " << std::filesystem::current_path() << std::endl;
	// Shader
	Shader ourShader("./src/vertexShader.glsl", "./src/fragmentShader.glsl");
	float screenHeight = 9.0f;
	float aspect = 16.0f / 9.0f;
	float screenWidth = screenHeight * aspect;

	glm::mat4 proj = glm::ortho(
		-screenWidth * 0.5f, screenWidth * 0.5f,
		-screenHeight * 0.5f, screenHeight * 0.5f,
		-1.0f, 1.0f
	);

	float worldWidth = screenWidth * 2.0f;
	float worldHeight = screenHeight * 2.0f;

	ourShader.use();
	ourShader.setMat4("uProj", proj);

	float lastTime = (float)glfwGetTime();
	
	Player player;
	PlayerInput playerInput;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float now = (float)glfwGetTime();
		float dt = now - lastTime;
		lastTime = now;

		playerInput.move(window, player.position, player.speed, dt);

		// --- CAMERA LOGIC ---
		float halfW = screenWidth / 2.0f;
		float halfH = screenHeight / 2.0f;
		float camX = player.position.x;
		float camY = player.position.y;

		camX = glm::clamp(camX, -worldWidth * 0.5f + halfW, worldWidth * 0.5f - halfW);
		camY = glm::clamp(camY, -worldHeight * 0.5f + halfH, worldHeight * 0.5f - halfH);

		std::cout << "Player: (" << player.position.x << ", " << player.position.y << ") ";
		std::cout << "Camera: (" << camX << ", " << camY << ") ";
		std::cout << "World: (" << worldWidth << ", " << worldHeight << ")\n";

		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camX, -camY, 0.0f));
		ourShader.setMat4("uView", view);

		glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		player.draw(ourShader);

		/* Render here */
		// glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
