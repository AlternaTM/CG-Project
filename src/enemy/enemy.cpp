#include "enemy.h"
#include "resourceManager/resourceManager.h"
#include <random>
#include "collision/collision.h"
#include "SkeletonEnemy.h"
#include "MageEnemy.h"
#include "mageBullet/mageCast.h"
#include <glm/gtc/matrix_transform.hpp>

Player* EnemyManager::_PLAYER = nullptr;


//  ============= MovingState ==================================


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

    //std::cout << CollisionChecker::distance(e, *EnemyManager::_PLAYER) << std::endl;
    
    if (CollisionChecker::distance(e, *EnemyManager::_PLAYER) < e.get_attack_distance()) {
        e.on_target_in_range(); 
    }

    flipped = direction.x < 0.0f;
    update_anim(dt);
}


//  ============= AttachingState ==================================


void MeleeAttackState::enter(Enemy& e) {
    reset_anim();
    if (EnemyManager::_PLAYER->get_pos()->x < e.get_pos()->x)
        flipped = true;
    else
        flipped = false;
}

void MeleeAttackState::update(Enemy& e,float dt) {
    if (CollisionChecker::distance(e, *EnemyManager::_PLAYER) >= e.get_attack_distance()) {
        e.on_target_out_of_range();
    }

    update_anim(dt);
    if (actual_frame == 6 && last_frame != 6) {
        EnemyManager::_PLAYER->hit(e.get_base_damage());
    }

     last_frame = actual_frame;
}

void MeleeAttackState::exit(Enemy& e) {

}


// ============= WaitingState ==================================

WaitingState::WaitingState(float time_to_waite, EnemyState* sta) {
    time_to_wait = time_to_waite;
    state_to_go = sta;
}


void WaitingState::enter(Enemy& e) {
    timer = 0;
    if (follow_player_dir_at_start) {
        if (EnemyManager::_PLAYER->get_pos()->x < e.get_pos()->x) {
            flipped = true;
        }
        else {
            flipped = false;
        }
    }
}

void WaitingState::exit(Enemy& e) {

}

void WaitingState::update(Enemy& e, float dt) {
    timer += dt;
    if (follow_player_dir) {
        if (EnemyManager::_PLAYER->get_pos()->x < e.get_pos()->x) {
            flipped = true;
        }
        else {
            flipped = false;
        }
    }

    update_anim(dt);
    if (timer > time_to_wait) {
        e.change_state(state_to_go);
    }

}

//  ============= ENEMY ==================================


Enemy::Enemy() {
    static uint32_t count = 0;
    ID = count++;

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

void Enemy::change_state(EnemyState* new_state) {
    currentState->exit(*this);
    currentState = new_state;
    currentState->enter(*this);
}

void Enemy::update( float dt) {
    currentState->update(*this, dt);
}

uint8_t Enemy::get_life() {
    return life;
}

glm::vec2 Enemy::get_frame_size() {
    return currentState->get_frame_size();
}

glm::vec2 Enemy::get_offset() {
    return currentState->get_offset();
}


// ============= ENEMY MANAGER ==================================


EnemyManager* EnemyManager::_INSTACE = nullptr;


EnemyManager* EnemyManager::get_instance() {
    if (_INSTACE == nullptr) {
        _INSTACE = new EnemyManager();
    }
    return _INSTACE;
}

void EnemyManager::spawn_enemy(EnemyTipe type, int n) {

    for (int i = 0; i < n; i++) {
        Enemy* e = nullptr;
        switch (type) {
            case EnemyTipe::Skeleton: e = new SkeletonEnemy(); break;
            case EnemyTipe::Mage: e = new MageEnemy(); break;
        }
        

        if (e != nullptr)
            enemys.push_back(e);

    }
        
}

void EnemyManager::remove_enemy(uint32_t ID) {
    auto it = std::find_if(enemys.begin(), enemys.end(),
        [ID](Enemy* e) { return e->ID == ID; });

    if (it != enemys.end()) {
        delete* it;
        enemys.erase(it);
    }
}

void EnemyManager::render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera) {
    for ( Enemy* e : enemys) {
        renderer.Draw(
            ResourceManager::GetTexture(e->get_texture_name()),
            *e->get_pos(),
            *e->get_size(),
            0.0f,
            camera.getViewMatrix(),
            e->get_offset(),
            e->get_frame_size()
        );
        drawlife(figRenderer, camera, *e->get_pos(), e->get_life());
    }

}

void EnemyManager::drawlife(FigRenderer& figRenderer, Camera& camera, const glm::vec2& pos,const uint8_t life) {
    glm::vec2 fPos = pos + glm::vec2(-0.25f,0.3f);

    float bar = life / 255.0f;
    bar *= 0.5f;

    std::vector<Vertex2D> vertices = {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f}, {0.0f, 1.0f}},
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };

    Mesh2D mesh;
    mesh.create(vertices, indices);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(fPos.x + 0.25f, fPos.y, 0.0f));
    //model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(bar, 0.05f, 1.0f));
    
    figRenderer.draw(mesh, model, camera.getViewMatrix());

    //figRenderer.drawRect(fPos, glm::vec2(bar, 0.05f),0,camera.getViewMatrix());
}

void EnemyManager::update(Player& player, float delta) {
    for (Enemy* e : enemys) {
        e->update(delta);
    }
}






