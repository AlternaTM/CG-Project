#pragma once
#include "camera/camera.h"
#include "spriteRenderer/spriteRenderer.h"
#include <vector>
#include "collision/collision.h"
#include "figureRenderer/figureRenderer.h"
#include "animSystem/animSystem.h"
#include "entity.h"
#include <glm/glm.hpp>


class Player;
class Enemy;

// ============= ENEMY STATES ==================================


const float ENEMY_SPEED = 1;

enum EnemyTipe
{
    Skeleton,
    Mage,
    Astro
};

class EnemyState : public Renderable{
public: 
    virtual ~EnemyState() = default;
    virtual void enter(Enemy&) {}
    virtual void update(Enemy&, float dt) = 0;
    virtual void exit(Enemy&) {}
    virtual glm::vec2 get_offset() {
        return {0.0f,0.0f};
    };
    virtual glm::vec2 get_frame_size() {
        return { 1.0f,1.0f };
    };
};


class MovingState : public EnemyState, public Animable {
public: 
    virtual void enter(Enemy&) override;
    virtual void update(Enemy&, float dt) override;
    virtual void exit(Enemy&) override;
    void on_animation_end() override {}
    glm::vec2 get_frame_size() override {
        return Animable::get_frame_size(); 
    }
    glm::vec2 get_offset() override {
        return Animable::get_offset();
    }
};

class WaitingState : public EnemyState, public Animable {
private: 
    float timer = 0.0f; 
    float time_to_wait = 0.0f;
    EnemyState* state_to_go;
    
public:
    WaitingState(float time_to_wait, EnemyState* state);

    bool follow_player_dir = false;
    bool follow_player_dir_at_start = false;

    virtual void enter(Enemy&) override;
    virtual void update(Enemy&, float dt) override;
    virtual void exit(Enemy&) override;
    void on_animation_end() override {}
    glm::vec2 get_frame_size() override {
        return Animable::get_frame_size();
    }
    glm::vec2 get_offset() override {
        return Animable::get_offset();
    }

    float get_timer() {
        return timer;
    }
    float get_time_to_wait() {
        return time_to_wait;
    }
};



class MeleeAttackState : public EnemyState, public Animable {
    uint8_t last_frame = 0;
public:
    virtual void enter(Enemy&) override;
    virtual void update(Enemy&, float dt) override;
    virtual void exit(Enemy&) override;
    void on_animation_end() override {}
    glm::vec2 get_frame_size() override {
        return Animable::get_frame_size();
    }
    glm::vec2 get_offset() override {
        return Animable::get_offset();
    }
};



// ============= ENEMY ==================================

class Enemy : public Renderable, public Entity {
protected:
    EnemyState* currentState = nullptr;

    uint8_t life = 255;
    
public:
    uint32_t ID;
    EnemyTipe type;
    virtual float       get_attack_distance() const = 0;
    virtual uint8_t     get_base_damage()     const = 0;
    virtual std::string get_texture_name()    const = 0;
    virtual void init_states() = 0;
    virtual void change_state(EnemyState* new_state);


    Enemy(EnemyTipe type);

    void make_damage(uint8_t damage);
    void update(float dt);
    uint8_t get_life();



    glm::vec2 get_offset() override;
    glm::vec2 get_frame_size() override;

    virtual void on_target_in_range() = 0;
    virtual void on_target_out_of_range() = 0;

    virtual ~Enemy() = default;
};


// ============= ENEMY MANAGER ==================================


class EnemyManager {
private:
    EnemyManager() {}
    static EnemyManager* _INSTACE;
    
    std::vector<Enemy *> enemys;
    void drawlife(FigRenderer& figRenderer, Camera& camera,const glm::vec2& pos, const uint8_t life);
public: 
    static Player* _PLAYER;
    static EnemyManager* get_instance();
    void spawn_enemy(EnemyTipe type, int n);
    void remove_enemy(uint32_t ID);
    void update(Player& player,float delta);
    void render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera);

};


