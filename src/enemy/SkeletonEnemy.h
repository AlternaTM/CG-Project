#pragma once
#include "enemy.h"


class SkeletonEnemy : public Enemy {
    MovingState movingState;
    MeleeAttackState attackState;
public:
    SkeletonEnemy();
    float       get_attack_distance() const override { return 0.9f; }
    uint8_t     get_base_damage()     const override { return 5; }
    std::string get_texture_name()    const override { return "skeleton"; }
    void        init_states()               override;
    void        on_target_in_range()        override;
    void        on_target_out_of_range()    override;
};