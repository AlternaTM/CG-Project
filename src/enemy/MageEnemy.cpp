#include "MageEnemy.h"

#include "SkeletonEnemy.h"




// ============ RangedAttackState

void RangedAttackState::enter(Enemy& e) {
    owner = &e;
    reset_anim();
}

void RangedAttackState::update(Enemy& e,float dt) {
    update_anim(dt);
}

void RangedAttackState::exit(Enemy& e) {
    owner = nullptr;
}


void RangedAttackState::on_animation_end() {
    if(owner)
        owner->on_target_out_of_range();
}
// ====================================

MageEnemy::MageEnemy() {
    init_states();
    life = 255;
}

void MageEnemy::init_states() {
    attackState.tot_framex = 8;
    attackState.tot_rows = 3;
    attackState.frame_duration = 0.15f;
    attackState.y_offset = 2;
    attackState.max_frame = 8;


    movingState.tot_framex = 8;
    movingState.tot_rows = 3;
    movingState.frame_duration = 0.3f;
    movingState.y_offset = 0;
    movingState.max_frame = 8;


    currentState = &movingState;
}

void MageEnemy::on_target_in_range() { change_state(&attackState); }
void MageEnemy::on_target_out_of_range() { change_state(&movingState); }