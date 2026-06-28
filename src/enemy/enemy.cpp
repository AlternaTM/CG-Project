#include "enemy.h"
#include "resourceManager/resourceManager.h"
#include <random>
#include "collision/collision.h"
#include "SkeletonEnemy.h"
#include "MageEnemy.h"
#include "astroEnemy.h" 
#include "mageBullet/mageCast.h"
#include "game/game.h"
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
        
        EnemyManager::get_instance()->playSound("assets/audio/swipe.wav");

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


Enemy::Enemy(EnemyTipe type):type(type) {
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
    float t = glm::clamp(Game::get_instance()->get_timer()->getTime() / (15.0f * 60.0f), 0.0f, 1.0f);
    float scaled =  glm::mix((float)damage, (float)(damage * MAX_DAMAGE_MULTI),t);

    life -= (uint8_t)damage;

}

void Enemy::change_state(EnemyState* new_state) {
    currentState->exit(*this);
    currentState = new_state;
    currentState->enter(*this);
}

void Enemy::update( float dt) {
    currentState->update(*this, dt);
}

int16_t Enemy::get_life() {
    return life;
}

glm::vec2 Enemy::get_frame_size() {
    return currentState->get_frame_size();
}

glm::vec2 Enemy::get_offset() {
    return currentState->get_offset();
}


// ============= ENEMY MANAGER ==================================


EnemyManager* EnemyManager::_INSTANCE = nullptr;

EnemyManager* EnemyManager::get_instance() {
    if (_INSTANCE == nullptr) {
        _INSTANCE = new EnemyManager();
        _INSTANCE->init_life_bar_mesh();
    }
    return _INSTANCE;
}

void EnemyManager::init_life_bar_mesh() {
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

    lifeBarMesh.create(vertices, indices);
}

void EnemyManager::spawn_enemy(EnemyTipe type, int n) {

    for (int i = 0; i < n; i++) {
        Enemy* e = nullptr;
        switch (type) {
            case EnemyTipe::Skeleton: e = new SkeletonEnemy(); break;
            case EnemyTipe::Mage: e = new MageEnemy(); break;
            case EnemyTipe::Astro: e = new AstroEnemy(); break;
        }
        

        if (e != nullptr)
            enemys.push_back(e);

    }

    auto priority = [](EnemyTipe type) -> int {
        switch (type) {
        case Astro:    return 0;
        case Skeleton: return 1;
        case Mage:     return 2;
        default:       return 99;
        }
    };

    std::sort(enemys.begin(), enemys.end(), [&priority](Enemy* a, Enemy* b) {
        return priority(a->type) < priority(b->type);
    });
}

/*
void EnemyManager::remove_enemy(uint32_t ID) {
    auto it = std::find_if(enemys.begin(), enemys.end(),
        [ID](Enemy* e) { return e->ID == ID; });

    if (it != enemys.end()) {
        delete* it;
        enemys.erase(it);
    }
}
*/

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

void EnemyManager::drawlife(FigRenderer& figRenderer, Camera& camera, const glm::vec2& pos,const int16_t life) {
    glm::vec2 fPos = pos + glm::vec2(-0.25f,0.3f);

    float bar = life / 255.0f;
    bar *= 0.5f;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(fPos.x + 0.25f, fPos.y, 0.0f));
    model = glm::scale(model, glm::vec3(bar, 0.05f, 1.0f));
    
    figRenderer.draw(lifeBarMesh, model, camera.getViewMatrix());
}

void EnemyManager::update(Player& player, float delta) {
    for (Enemy* e : enemys) {
        e->update(delta);
    }
    fix_overlaps();

    enemys.erase(
        std::remove_if(enemys.begin(), enemys.end(), [this, &player](Enemy* e) {
            if (e->get_life() <= 0) {
                if (e->type == EnemyTipe::Astro) {
                    Game::get_instance()->get_chestManager().spawn_chest();
                }
                Game::get_instance()->updateScoreCount(e->get_reward_score());
                delete e; 
                return true;
            }
            return false;
            }),
        enemys.end()
    );


    spawnTimer += delta;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;

        if (enemys.size() < minEnemies) {
            for (uint16_t i = 0; i < randomAmmountSpawn(); i++) {
                spawn_enemy(randomWeightedEnemyType(), 1); 
            }
        }
    }
}


void EnemyManager::fix_overlaps() {
    for (int i = 0; i < enemys.size(); i++) {
        for (int y = i+1; y < enemys.size(); y++) {
            float dx = enemys[y]->get_pos()->x - enemys[i]->get_pos()->x;
            float dy = enemys[y]->get_pos()->y - enemys[i]->get_pos()->y;

            float dist = sqrtf(dx * dx + dy * dy);



            if (dist < 0.3f && dist > 0) {
                float overlap = 0.3f / 2.0f * 0.1f;
                float nx = dx / dist;
                float ny = dy / dist;
                if (enemys[i]->type != EnemyTipe::Astro) {
                    enemys[i]->get_pos()->x -= nx * overlap;
                    enemys[i]->get_pos()->y -= ny * overlap;
                }
                if (enemys[y]->type != EnemyTipe::Astro) {
                    enemys[y]->get_pos()->x += nx * overlap;
                    enemys[y]->get_pos()->y += ny * overlap;
                }
            }

        }
    }
}



EnemyTipe EnemyManager::randomWeightedEnemyType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    
    static std::discrete_distribution<int> dist({ 60, 30, 10 });

    return static_cast<EnemyTipe>(dist(gen));
}

uint16_t EnemyManager::randomAmmountSpawn() {
    static std::random_device rd;
    static std::mt19937 gen(rd());



    float t = clamp(Game::get_instance()->get_timer()->getTime() / (15.0f * 60.0f), 0.0f, 1.0f);

    float bias = t * t * t;


    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float raw = lerp(static_cast<float>(spawnAmountMin),
        static_cast<float>(spawnAmmoutnMax),
        bias + (dist(gen) * 0.2f - 0.1f)); 

    return static_cast<uint16_t>(
        clamp(static_cast<float>(std::round(raw)),
            static_cast<float>(spawnAmountMin),
            static_cast<float>(spawnAmmoutnMax))
        );
}

void EnemyManager::reset() {
    enemys.erase(
        std::remove_if(enemys.begin(), enemys.end(), [](Enemy* e) {
                delete e;
                return true;
            }),
        enemys.end()
    );
    spawnTimer = 0.0f;
}


void EnemyManager::init(irrklang::ISoundEngine* engine) {
    audioEngine = engine;
}

void EnemyManager::playSound(const std::string& file) {
    if (audioEngine != nullptr) {
        audioEngine->play2D(file.c_str());
    }
    
}


irrklang::ISound* EnemyManager::playCostantSound(const std::string& file) {
    return audioEngine->play2D(file.c_str(), true, false, true);
}


void EnemyManager::stopConstantSound(irrklang::ISound* isound) {
    if (isound) {
        isound->stop();
        isound->drop();  
        isound = nullptr;
    }
}



