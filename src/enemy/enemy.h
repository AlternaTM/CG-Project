#pragma once
#include "camera/camera.h"
#include "spriteRenderer/spriteRenderer.h"
#include <vector>
#include "collision/collision.h"
#include "figureRenderer/figureRenderer.h"
#include "animSystem/animSystem.h"
#include "entity.h"

class Player;


enum class EnemyState {
    Moving,
    Attacking
};

const float ENEMY_SPEED = 1;

class Enemy : public Renderable, public Entity {
private: 
    uint8_t life;
public:
    uint32_t ID;
    EnemyState state;
    Enemy();
    void make_damage(uint8_t damage);
    uint8_t get_life();

    glm::vec2 get_offset() override;
    glm::vec2 get_frame_size() override;
};


class EnemyManager {
private:
    EnemyManager() {}
    static EnemyManager* _INSTACE;
    std::vector<Enemy> enemys;
    void drawlife(FigRenderer& figRenderer, Camera& camera,const glm::vec2& pos, const uint8_t life);
    
public: 
    static EnemyManager* get_instance();
    void spawn_enemy(int n);
    void remove_enemy(uint32_t ID);
    void update(Player& player,float delta);
    void render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera);

};


