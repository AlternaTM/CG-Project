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


class AttachingState : public EnemyState, public Animable {
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
private: 
    uint8_t life;

public:
    float ATTACK_DISTANCE = 0.9f;
    MovingState movingState;
    AttachingState attachingState;
    EnemyState* currentState;
    uint32_t ID;
    Enemy();

    void make_damage(uint8_t damage);
    void update(float dt);
    uint8_t get_life();

    void change_state(EnemyState*);

    Enemy& operator=(const Enemy& other);
    Enemy& operator=(Enemy&& other) noexcept;

    glm::vec2 get_offset() override;
    glm::vec2 get_frame_size() override;
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
    void spawn_enemy(int n);
    void remove_enemy(uint32_t ID);
    void update(Player& player,float delta);
    void render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera);

};


