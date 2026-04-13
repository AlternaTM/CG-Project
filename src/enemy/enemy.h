#pragma once
#include "camera/camera.h"
#include "spriteRenderer/spriteRenderer.h"
#include <vector>
#include "collision/collision.h"
#include "figureRenderer/figureRenderer.h"

enum class EnemyState {
    Moving,
    Attacking
};

const float ENEMY_SPEED = 1;

class Enemy {
public:
    EnemyState state;
    glm::vec3 position;
    glm::vec2 size;
    Enemy();

};


class EnemyManager {
private:
    std::vector<Enemy> enemys;
    void drawlife(FigRenderer& figRenderer, Camera& camera,const glm::vec2& pos);
    
public: 
    void spawn_enemy(int n);
    void update(Player& player,float delta);
    void render(SpriteRenderer& renderer, FigRenderer& figRenderer, Camera& camera);
};


