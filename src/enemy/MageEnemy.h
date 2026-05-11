#pragma once
#include "enemy.h"


class RangedAttackState : public EnemyState, public Animable {
private:
    Enemy* owner = nullptr;
    uint8_t last_frame = 0;
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


class MageEnemy : public Enemy {
    MovingState movingState;
    RangedAttackState attackState;
public:
    MageEnemy();
    float       get_attack_distance() const override { return 0.9f; }
    uint8_t     get_base_damage()     const override { return 5; }
    std::string get_texture_name()    const override { return "mago"; }
    void        init_states()               override;
    void        on_target_in_range()        override;
    void        on_target_out_of_range()    override;
};