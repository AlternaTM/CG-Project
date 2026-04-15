#include "enemy.h"
#include "resourceManager/resourceManager.h"
#include <random>

Enemy::Enemy() {
    static uint32_t count = 0;
    ID = count++;

	state = EnemyState::Moving;
    life = 255;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-5.0f, 5.0f);

    position = glm::vec3(
        dist(gen),
        dist(gen),
        0.0f
    );
    size = glm::vec2(1.0f,1.0f);
}

void Enemy::make_damage(uint8_t damage) {
    life -= damage;
    if (life <= 0)
        EnemyManager::get_instance()->remove_enemy(this->ID); //QUI MUORE L'OGGETTO; NON FARE NULLA DOPO QUESTA RIGA
}

uint8_t Enemy::get_life() {
    return life;
}

EnemyManager* EnemyManager::_INSTACE = nullptr;


EnemyManager* EnemyManager::get_instance() {
    if (_INSTACE == nullptr) {
        _INSTACE = new EnemyManager();
    }
    return _INSTACE;
}

void EnemyManager::spawn_enemy(int n) {

    for (int i = 0; i < n; i++) {
        enemys.emplace_back();

    }
        
}

void EnemyManager::remove_enemy(uint32_t ID) {
    enemys.erase(
        std::remove_if(enemys.begin(), enemys.end(),
            [ID](const Enemy& e) { return e.ID == ID; }),
        enemys.end()
    );
}

void EnemyManager::render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera) {
    for ( Enemy& e : enemys) {
        renderer.Draw(
            ResourceManager::GetTexture("enemy"),
            e.position,
            e.size,
            0.0f,
            camera.getViewMatrix()
        );
        drawlife(figRenderer, camera,e.position, e.get_life());
    }

    
}

void EnemyManager::drawlife(FigRenderer& figRenderer, Camera& camera, const glm::vec2& pos,const uint8_t life) {
    glm::vec2 fPos = pos + glm::vec2(-0.5,0.6f);

    float bar = life / 255.0f;

    figRenderer.drawRect(fPos, glm::vec2(bar, 0.2f),camera.getViewMatrix());
}





void EnemyManager::update(Player& player, float delta) {
    for (Enemy& e : enemys) {
        if (e.state == EnemyState::Moving) {
            glm::vec2 direction = *(player.get_pos()) - e.position;
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




