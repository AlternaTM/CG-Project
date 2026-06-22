#include "chest.h"
#include "resourceManager/resourceManager.h"
#include <random>



Chest::Chest(float x, float y) 
{
    Entity::get_pos()->x = x;
    Entity::get_pos()->y = y;
    Entity::get_size()->x = 0.50f;
    Entity::get_size()->y = 0.50f;
}


void ChestManager::render(SpriteRenderer& renderer, Camera& camera) {

    for (size_t i = 0; i < chests.size(); i++) {
        renderer.Draw(
            ResourceManager::GetTexture("chest"),
            *chests[i].get_pos(),
            *chests[i].get_size(),
            0.0f,
            camera.getViewMatrix()
        );
    }
}


bool ChestManager::interact(GLFWwindow* window, Player& pl) {
    
    if (PlayerInput::isKeyJustPressed(window, GLFW_KEY_E)) {
        for (size_t i = chests.size(); i-- > 0;) {
            if (CollisionChecker::check_collision(pl, chests[i])) {
                chests.erase(chests.begin() + i);
                //pl.set_state(LootingState::instance());
                return true;
            }
        }
    }
    return false;
}


ChestManager::ChestManager() {


}



void ChestManager::render_chest(Camera3D& camera3D, const glm::mat4& projection3D, ModelRenderer& chest, ModelRenderer& chest_lid, float dt) {
    static float rotate = 0.0f;
    static glm::vec2 pos = { 0.57f,0.0f };
    if (angle > 95.0f) {
        finished = true;
    }else {
        angle += 40.0f * dt;
        
    }
    

    float t = angle / 95.0f;



    pos.x = glm::mix(0.65f, 0.85f, t);
    pos.y = glm::mix(0.0f, 0.90f, t);

    rotate = glm::mix(0.0f, 360.0f, t);

    chest.setRotation(glm::vec3(0, rotate, 0));

    chest_lid.setPosition(glm::vec3(0, pos.x, -pos.y)); //0.0 - 0.95f - -0.85f
    //chest_lid.setPosition(glm::vec3(0, 0.0, 0.0));

    chest_lid.setRotation(glm::vec3(-angle, 0.0f, 0));

    glm::mat4 view = camera3D.getViewMatrix();

    //glm::mat4 view = glm::lookAt(camera3D.position, camera3D.target, camera3D.up);

    Shader& shader = *chest.shader;
    shader.use();
    shader.setVec3("viewPos", camera3D.position);

    // Luce principale
    shader.setVec3("lightPos1", glm::vec3(2, 2, 2));
    shader.setVec3("lightColor1", glm::vec3(1, 1, 1));

    // Luce sopra-destra
    shader.setVec3("lightPos2", glm::vec3(5, 8, 5));
    shader.setVec3("lightColor2", glm::vec3(0.6f, 0.6f, 0.6f));


    // Luce dentro
    shader.setVec3("lightInside", glm::vec3(0, 0.5f, 0));
    shader.setVec3("lightColorInside", glm::vec3(6.0f, 4.0f, 0.0f));

    shader.setFloat("attLinear", 0.09f);
    shader.setFloat("attQuadratic", 0.032f);

    glm::mat4 chestWorld = chest.getModelMatrix();

    chest.render(camera3D.getViewMatrix(), projection3D);
    chest_lid.render(camera3D.getViewMatrix(), projection3D, &chestWorld);

}


void ChestManager::spawn_chest() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-5.0f, 5.0f);

    chests.push_back(
        Chest(dist(gen), dist(gen))
    );
}


void ChestManager::reset() {
    chests.clear();
}