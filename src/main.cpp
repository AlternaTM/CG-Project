#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader/shader.h"
#include "player/playerInput.h"
#include "player/player.h"
#include "world/world.h"
#include "camera/camera.h"
#include "resourceManager/resourceManager.h"
#include "spriteRenderer/spriteRenderer.h"

//#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float SCREEN_HEIGHT = 9.0f;
float ASPECT = 16.0f / 9.0f;
float SCREEN_WIDTH = SCREEN_HEIGHT * ASPECT;

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

	/* Capturing mouse if it stays inside the window */
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader spriteShader(
        "./src/vertexShader.glsl",
        "./src/fragmentShader.glsl"
    );

    glm::mat4 projection = glm::ortho(
        -SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f,
        -SCREEN_HEIGHT * 0.5f, SCREEN_HEIGHT * 0.5f,
        -1.0f, 1.0f
    );

    spriteShader.use();
    spriteShader.setMat4("uProj", projection);
    spriteShader.setInt("uTexture", 0);

    Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

    float worldWidth = SCREEN_WIDTH * 2.0f;
    float worldHeight = SCREEN_HEIGHT * 2.0f;
    camera.setWorldBounds(worldWidth, worldHeight);

    // Inizializzo il renderer
    SpriteRenderer renderer(spriteShader);

    // Carico le textures
    ResourceManager::LoadTexture("player", "./assets/teddybear.png");
    ResourceManager::LoadTexture("world", "./assets/wood.jpeg");

    // ------------------- Player -------------------
    Player player;

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        PlayerInput::move(window, player.position, player.speed, dt);
        PlayerInput::updateMouse(window, player.mousePosition);

        camera.follow(player.position);

        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.Draw(
            ResourceManager::GetTexture("world"),
            { 0.0f, 0.0f },              // centro
            { worldWidth, worldHeight },
            0.0f,
            camera.getViewMatrix()
        );

        renderer.Draw(
            ResourceManager::GetTexture("player"),
            player.position,
            { 1.0f, 1.0f },
            0.0f,
            camera.getViewMatrix()
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
