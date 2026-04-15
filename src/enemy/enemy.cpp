#include "enemy.h"
#include "resourceManager/resourceManager.h"
#include <random>

Player* EnemyManager::_PLAYER = nullptr;

//  ============= MovingState ==================================

MovingState* MovingState::instance() {
    static MovingState inst;
    return &inst;
}

void MovingState::enter(Enemy& e) {

}

void MovingState::exit(Enemy& e) {

}

void MovingState::update(Enemy& e, float dt) {
    glm::vec2 direction = (*EnemyManager::_PLAYER->get_pos()) - (*e.get_pos());
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
    }

    e.get_pos()->x += direction.x * ENEMY_SPEED * dt;
    e.get_pos()->y += direction.y * ENEMY_SPEED * dt;


    
}

//  ============= AttachingState ==================================

AttachingState* AttachingState::instance() {
    static AttachingState inst; 
    return &inst;
}

void AttachingState::enter(Enemy& e) {

}

void AttachingState::update(Enemy& e,float dt) {

}

void AttachingState::exit(Enemy& e) {

}




//  ============= ENEMY ==================================


Enemy::Enemy() {
    static uint32_t count = 0;
    ID = count++;

	//state = EnemyState::Moving;
    currentState = MovingState::instance();
    life = 255;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-5.0f, 5.0f);

 
    *Entity::get_pos() = glm::vec3(
        dist(gen),
        dist(gen),
        0.0f
    );
    *Entity::get_size() = glm::vec2(1.0f, 1.0f);
}

void Enemy::make_damage(uint8_t damage) {
    life -= damage;
    if (life <= 0)
        EnemyManager::get_instance()->remove_enemy(this->ID); //QUI MUORE L'OGGETTO; NON FARE NULLA DOPO QUESTA RIGA
}


void Enemy::update(Enemy& e, float dt) {
    currentState->update(e, dt);
}

uint8_t Enemy::get_life() {
    return life;
}

glm::vec2 Enemy::get_frame_size() {
    return { 1.0f,1.0f };
}

glm::vec2 Enemy::get_offset() {
    return { 0.0f,0.0f };
}


// ============= ENEMY MANAGER ==================================


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
            *e.get_pos(),
            *e.get_size(),
            0.0f,
            camera.getViewMatrix(),
            e.get_offset(),
            e.get_frame_size()
        );
        drawlife(figRenderer, camera, *e.get_pos(), e.get_life());
    }

    
}

void EnemyManager::drawlife(FigRenderer& figRenderer, Camera& camera, const glm::vec2& pos,const uint8_t life) {
    glm::vec2 fPos = pos + glm::vec2(-0.5,0.6f);

    float bar = life / 255.0f;

    figRenderer.drawRect(fPos, glm::vec2(bar, 0.2f),camera.getViewMatrix());
}





void EnemyManager::update(Player& player, float delta) {
    for (Enemy& e : enemys) {

        e.update(e, delta);
        
    }
}






