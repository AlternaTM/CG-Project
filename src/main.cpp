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
#include "textRenderer/textRenderer.h"
#include "timer/timer.h"

#include "models/model.h"

#include <irrKlang/irrKlang.h>

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

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader spriteShader(
        "src/glsl/spriteVertexShader.glsl",
        "src/glsl/spriteFragShader.glsl"
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
    ResourceManager::LoadTexture("player", "assets/textures/teddybear.png");
    ResourceManager::LoadTexture("world", "assets/textures/wood.jpeg");
    ResourceManager::LoadTexture("arrow", "assets/textures/arrow.png");

    // Avvio motore audio
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine)
        return 0; // error starting up the engine

    // Avvio BGM looped (da cambiare la musica)
    engine->play2D("assets/audio/bgm.wav", true);

    // ------------------- Player -------------------
    Player player(camera);

    Timer timer;
    TextRenderer textRenderer(SCR_WIDTH, SCR_HEIGHT);

    textRenderer.LoadFont("./assets/fonts/PressStart2P-Regular.ttf", 48);

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        player.update(dt, window);
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

        renderer.Draw(
            ResourceManager::GetTexture("arrow"),
            player.mousePosition,
            { 1.0f, 1.0f },     
            player.rotation,    
            camera.getViewMatrix()
        );

        timer.update(dt);

        int min = timer.getMinutes();
        int sec = timer.getSeconds();

        char buffer[6];
        sprintf_s(buffer, "%02d:%02d", min, sec);

        textRenderer.RenderText(buffer, 20.0f, SCR_HEIGHT - 70.0f, 0.7f, { 1.0f, 1.0f, 1.0f });

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //----------------chest---------------------

    //Model chestModel("assets/models/chest.obj");




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
