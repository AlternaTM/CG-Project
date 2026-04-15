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


class EnemyState {
public: 
    virtual ~EnemyState() = default;
    virtual void enter(Enemy&) {}
    virtual void update(Enemy&, float dt) = 0;
    virtual void exit(Enemy&) {}
};


class MovingState : public EnemyState {
public: 
    static MovingState* instance();
    virtual void enter(Enemy&) override;
    virtual void update(Enemy&, float dt) override;
    virtual void exit(Enemy&) override;
};


class AttachingState : public EnemyState {
public:
    static AttachingState* instance();
    virtual void enter(Enemy&) override;
    virtual void update(Enemy&, float dt) override;
    virtual void exit(Enemy&) override;
};

// ============= ENEMY ==================================

class Enemy : public Renderable, public Entity {
private: 
    uint8_t life;

public:
    float ATTACK_DISTANCE = 0.9f;
    EnemyState* currentState;
    uint32_t ID;
    Enemy();
    void make_damage(uint8_t damage);
    void update(Enemy&, float dt);
    uint8_t get_life();

    void change_state(EnemyState*);

    glm::vec2 get_offset() override;
    glm::vec2 get_frame_size() override;
};





// ============= ENEMY MANAGER ==================================


class EnemyManager {
private:
    EnemyManager() {}
    static EnemyManager* _INSTACE;
    std::vector<Enemy> enemys;
    void drawlife(FigRenderer& figRenderer, Camera& camera,const glm::vec2& pos, const uint8_t life);
    
public: 
    static Player* _PLAYER;
    static EnemyManager* get_instance();
    void spawn_enemy(int n);
    void remove_enemy(uint32_t ID);
    void update(Player& player,float delta);
    void render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera);

};


