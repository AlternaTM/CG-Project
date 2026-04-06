#include "enemy.h"
#include "resourceManager/resourceManager.h"
#include <random>

Enemy::Enemy() {
	state = EnemyState::Moving;


    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-5.0f, 5.0f);

    position = glm::vec3(
        dist(gen),
        dist(gen),
        0.0f
    );
    size = glm::vec2(1.0f,1.0f);
}



void EnemyManager::spawn_enemy(int n) {
    for(int i =0; i<n;i++)
        enemys.emplace_back();
}

void EnemyManager::render(SpriteRenderer& renderer, Camera& camera) {
    for (const Enemy& e : enemys) {
        renderer.Draw(
            ResourceManager::GetTexture("enemy"),
            e.position,
            e.size,
            0.0f,
            camera.getViewMatrix()
        );
    }
}


void EnemyManager::update(Player& player, float delta) {
    for (Enemy& e : enemys) {
        if (e.state == EnemyState::Moving) {
            glm::vec2 direction = player.position - e.position;
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0.0f) {
                direction.x /= length;
                direction.y /= length;
            }

            e.position.x += direction.x * ENEMY_SPEED *delta;
            e.position.y += direction.y * ENEMY_SPEED *delta;
        }
    }
}