#include "MageEnemy.h"

#include "SkeletonEnemy.h"

#include "mageBullet/mageCast.h"


// ============ RangedAttackState

void RangedAttackState::enter(Enemy& e) {
    owner = &e;
    reset_anim();
    MageEnemy* me = dynamic_cast<MageEnemy*>(&e);
    if (me->saved_target.x < e.get_pos()->x) {
        flipped = true;
    }
    else {
        flipped = false;
    }
    CastManager::get_instance()->spawn(
        glm::vec2(e.get_pos()->x, e.get_pos()->y), 
        me->saved_target,
        glm::vec2(20.0f, 0.5f), 
        2.0f
    );
}

void RangedAttackState::update(Enemy& e,float dt) {
    MageEnemy* me = dynamic_cast<MageEnemy*>(&e);
    update_anim(dt);
}

void RangedAttackState::exit(Enemy& e) {
    owner = nullptr;
}


void RangedAttackState::on_animation_end() {
    if(owner)
        owner->on_target_out_of_range();
}

// ============ RangedAttackState

void PreAttackState::enter(Enemy& e) {
    WaitingState::enter(e);
    std::cout << "test" << std::endl;
    MageEnemy* em = dynamic_cast<MageEnemy*>(&e);
    em->saved_target = *EnemyManager::_PLAYER->get_pos();
}

// ====================================

MageEnemy::MageEnemy() 
    :preAttackState(2.0f,&attackState)
{
    init_states();
    life = 255;
}

void MageEnemy::init_states() {
    attackState.tot_framex = 8;
    attackState.tot_rows = 4;
    attackState.frame_duration = 0.15f;
    attackState.y_offset = 2;
    attackState.max_frame = 8;


    movingState.tot_framex = 8;
    movingState.tot_rows = 4;
    movingState.frame_duration = 0.3f;
    movingState.y_offset = 0;
    movingState.max_frame = 8;

    

    preAttackState.tot_framex = 8;
    preAttackState.tot_rows = 4;
    preAttackState.frame_duration = 0.15f;
    preAttackState.y_offset = 3;
    preAttackState.max_frame = 8;
    preAttackState.follow_player_dir_at_start = true;

    currentState = &movingState;
}

void MageEnemy::on_target_in_range() { change_state(&preAttackState); }
void MageEnemy::on_target_out_of_range() { change_state(&movingState); }