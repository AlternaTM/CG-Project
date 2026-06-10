#pragma once
#include "enemy.h"
#include "mageBullet/mageCast.h"

class MageEnemy;
// =============== STATE FOR MAGE =====================

class RangedAttackState : public EnemyState, public Animable {
private:
    Enemy* owner = nullptr;
    uint8_t last_frame = 0;
    float start_time;
    float ttl;
public:
    uint8_t start_pos;
    uint8_t end_pos;
    uint8_t duration;

    virtual void enter(Enemy&) override;
    virtual void update(Enemy&, float dt) override;
    virtual void exit(Enemy&) override;
    void on_animation_end() override;
    glm::vec2 get_frame_size() override {
        return Animable::get_frame_size();
    }
    glm::vec2 get_offset() override {
        return Animable::get_offset();
    }
};


class PreAttackState : public WaitingState {
private:
    float start_time;
public:
    PreAttackState(float time_to_wait, EnemyState* state) : WaitingState(time_to_wait, state) {

    }
    void enter(Enemy& e)    override;
    void update(Enemy&, float dt) override;
    void exit(Enemy& e)     override;
};



// =============== MAGE =====================

class MageEnemy : public Enemy {
private:
    RangedAttackState attackState;
    MovingState movingState;
    PreAttackState preAttackState;
    WaitingState endAttackDelay;
    Cast* cast;
public:
    MageEnemy();
    glm::vec2 saved_target;
    float       get_attack_distance() const override { return 3.5f; }
    uint8_t     get_base_damage()     const override { return 5; }
    std::string get_texture_name()    const override { return "mago"; }
    void        init_states()               override;
    void        on_target_in_range()        override;
    void        on_target_out_of_range()    override;


    void spawn_cast(glm::mat4 model, const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    void remove_cast();

    Cast* get_cast() {
        return cast;
    }
};