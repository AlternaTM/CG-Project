#include "chest.h"
#include "resourceManager/resourceManager.h"
#include <random>


Chest::Chest(float x, float y) 
:position(x,y,0),size(0.50f,0.50f) {
}


void ChestManager::render(SpriteRenderer& renderer, Camera& camera) {

    for (size_t i = 0; i < chests.size(); i++) {
        renderer.Draw(
            ResourceManager::GetTexture("chest"),
            chests[i].position,
            chests[i].size,
            0.0f,
            camera.getViewMatrix()
        );
    }
}


void ChestManager::interact(GLFWwindow* window, Player& pl) {
    if (PlayerInput::isKeyJustPressed(window, GLFW_KEY_E)) {
        for (size_t i = chests.size(); i-- > 0;) {
            if (CollisionChecker::check_collision(pl, chests[i])) {
                chests.erase(chests.begin() + i);
                pl.set_state(LootingState::instance());
            }
        }
    }
}


ChestManager::ChestManager(int n) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-5.0f, 5.0f);

    for (int i = 0; i < n; i++) {
        chests.push_back(
            Chest(dist(gen), dist(gen))
        );
    }

}