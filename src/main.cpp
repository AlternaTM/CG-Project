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



#include "chest/chest.h"
#include "models/model.h"
#include "modelRenderer/modelRenderer.h"
#include "collision/collision.h"
#include "camera/camera3D.h"
#include "models/modelHelper.h"
#include "enemy/enemy.h"
#include "figureRenderer/figureRenderer.h"
#include "enemy/mageBullet/mageCast.h"

#include <irrKlang/irrKlang.h>
#include <ft2build.h>
#include <vector>
#include <random>

#include "game/game.h"
#include "game/globals.h"

#include "models/modelBuilder/modelBuilder.h"

#include FT_FREETYPE_H

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
    glfwWindowHint(GLFW_DEPTH_BITS, 24);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Battlefield in the Bedroom", NULL, NULL);
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

    // ========== SETUP 2D ================
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader spriteShader(
        "shaders/glsl/spriteVertexShader.glsl",
        "shaders/glsl/spriteFragShader.glsl"
    );

    glm::mat4 projection = glm::ortho(
        -SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f,
        -SCREEN_HEIGHT * 0.5f, SCREEN_HEIGHT * 0.5f,
        -1.0f, 1.0f
    );

    spriteShader.use();
    spriteShader.setMat4("uProj", projection);
    spriteShader.setInt("uTexture", 0);






    // ========== SETUP 3D ===============

    glm::mat4 projection3D = glm::perspective(
        glm::radians(45.0f),
        (float)SCR_WIDTH / (float)SCR_HEIGHT,
        0.1f,
        100.0f
    );

    Camera3D camera3D(
        glm::vec3(0.0f, 2.0f, 8.0f), 
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    // ============= WORD AND CAMERA ========

    Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

    float worldWidth = SCREEN_WIDTH * 2.0f;
    float worldHeight = SCREEN_HEIGHT * 2.0f;
    camera.setWorldBounds(worldWidth, worldHeight);

    // Inizializzo il renderer
    SpriteRenderer renderer(spriteShader);

    // Carico le textures
    ResourceManager::LoadTexture("player", "assets/textures/teddyAnim.png");
    ResourceManager::LoadTexture("world", "assets/textures/wood.jpeg");
    ResourceManager::LoadTexture("chest", "assets/textures/chest.png");
    ResourceManager::LoadTexture("arrow", "assets/textures/arrow.png");
    ResourceManager::LoadTexture("skeleton", "assets/textures/skelSheet.png");
    ResourceManager::LoadTexture("mago", "assets/textures/magoSheet.png");
    ResourceManager::LoadTexture("cannon", "assets/textures/cannon.png");
    ResourceManager::LoadTexture("bullet", "assets/textures/proiettile_base.png");
    ResourceManager::LoadTexture("titleScreen", "assets/textures/title.png");

    // Avvio motore audio
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine)
        return 0; // error starting up the engine

    // Avvio BGM looped (da cambiare la musica)
    engine->setSoundVolume(0.3f);
    engine->play2D("assets/audio/bgm.wav", true);




    //----------------chest---------------------

    // Chest base

    Shader& chestShader = ResourceManager::LoadShader("chest",
        "shaders/glsl/modelVertexShader.glsl",
        "shaders/glsl/modelFragShader.glsl");

    
    ModelRenderer& chest = ResourceManager::LoadModel("chest", "assets/models/chest/chest.obj", &chestShader);
    chest.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    chest.setScale(glm::vec3(1.0f));

    // Chest lid
    ModelRenderer& chest_lid = ResourceManager::LoadModel("chest_lid", "assets/models/chest/chest_lid.obj", &chestShader);

    chest_lid.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    chest_lid.setScale(glm::vec3(1.0f));
    
    float chest_top = ModelHelper::getMaxY(chest);
    float lid_min_y = ModelHelper::getMinY(chest_lid);
    float lid_max_y = ModelHelper::getMaxY(chest_lid);
    float lid_min_z = ModelHelper::getMinZ(chest_lid);
    float lid_max_z = ModelHelper::getMaxZ(chest_lid);
    float lid_height = lid_max_y - lid_min_y;


    chest_lid.setPosition(glm::vec3(
        0.0f,
        chest_top + (lid_height / 4) ,  
        0.0f                           
    ));



    // ------------------- Orso -------------------

    Shader& bearShader = ResourceManager::LoadShader("bear",
        "shaders/bear_shader/bearVertexShader.glsl",
        "shaders/bear_shader/bearFragShader.glsl");


    ResourceManager::LoadModel("lamp","assets/models/lamp/lampada.obj", &bearShader);

    ResourceManager::LoadModel("orso", "assets/models/bear/Orso.obj", &bearShader);

    //------------------- PIANO -------------------

    ModelRenderer* plane = ModelBuilder::buildPlane(glm::vec2(20.0f,10.0f),"assets/models/plane/wood_floor_basecolor.png", &bearShader);
    ResourceManager::addModel("plane", plane);

    
    //asteroid 
    Shader& asterShader = ResourceManager::LoadShader("aster",
        "shaders/asteroid3d/ast3dVertex.glsl",
        "shaders/asteroid3d/ast3dFrag.glsl");

    ResourceManager::LoadModel("asteroid", "assets/models/astro/asteroid.obj", &asterShader);
    
    //------------------- GAME -------------------
    

    Game::init(
        window,
        camera,
        projection,
        &renderer,
        &camera3D,
        projection3D,
        engine
    );
    std::mt19937 rng(std::random_device{}());
    Game* game = Game::get_instance();

    float lastTime = (float)glfwGetTime();
    float now = 0.0f;
	float dt = 0.0f;



    //------------------- CICLO ------------------- 
    while (!glfwWindowShouldClose(window)) {
        now = (float)glfwGetTime();
        dt = now - lastTime;
        lastTime = now;

        
        game->update(dt);

        camera.follow(*game->get_player()->get_pos());



        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ============ RENDERING 2D ============
        glDisable(GL_DEPTH_TEST);

        game->render2d();

        // ============ RENDERING 3D ============
        glEnable(GL_DEPTH_TEST);
        
        game->render3d(dt);
        
        // RENDERUI 
        glDisable(GL_DEPTH_TEST);

        game->renderUI();
        //============= FINE RENDER LOOP ==============
        PlayerInput::update_input(window);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    
    ResourceManager::clearModels();
    ResourceManager::clearShaders();
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