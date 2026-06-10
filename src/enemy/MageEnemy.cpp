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

    me->spawn_cast(
        glm::vec2(e.get_pos()->x, e.get_pos()->y),
        me->saved_target,
        glm::vec2(20.0f, 0.5f),
        2.0f,
        glm::vec4(0.7f, 0.0f, 0.3f, 0.97f)
    );

}

void RangedAttackState::update(Enemy& e,float dt) {
    MageEnemy* me = dynamic_cast<MageEnemy*>(&e);
    update_anim(dt);
}

void RangedAttackState::exit(Enemy& e) {
    MageEnemy* me = dynamic_cast<MageEnemy*>(&e);
    me->remove_cast();
    owner = nullptr;
}


void RangedAttackState::on_animation_end() {
    if(owner)
        owner->on_target_out_of_range();
}



// ============ RangedAttackState

void PreAttackState::enter(Enemy& e) {
    WaitingState::enter(e);
    MageEnemy* em = dynamic_cast<MageEnemy*>(&e);
    em->saved_target = *EnemyManager::_PLAYER->get_pos();

    em->spawn_cast(
        glm::vec2(e.get_pos()->x, e.get_pos()->y),
        em->saved_target,
        glm::vec2(20.0f, 0.5f),
        30.0f,
        glm::vec4(1.0f, 0.0f, 0.0f, 0.3f)
    );
}
void PreAttackState::exit(Enemy& e) {
    WaitingState::exit(e);
    MageEnemy* em = dynamic_cast<MageEnemy*>(&e);
    em->remove_cast();
}


// ====================================

MageEnemy::MageEnemy() 
    :preAttackState(2.0f,&attackState), endAttackDelay(1.0f, &movingState), cast(nullptr)
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


    endAttackDelay.tot_framex = 8;
    endAttackDelay.tot_rows = 4;
    endAttackDelay.frame_duration = 0.15f;
    endAttackDelay.y_offset = 3;
    endAttackDelay.max_frame = 8;
    endAttackDelay.follow_player_dir_at_start = true;

    currentState = &movingState;

    size.x = 2.0f;
    size.y = 2.0f;
}

void MageEnemy::on_target_in_range() { change_state(&preAttackState); }
void MageEnemy::on_target_out_of_range() { change_state(&endAttackDelay); }



void MageEnemy::spawn_cast(glm::vec2 start, glm::vec2 target, glm::vec2 size, float ttl, const glm::vec4& color) {
    glm::vec2 dir = glm::normalize(target - start);

    float angle = atan2(dir.y, dir.x);
    remove_cast();

    cast = new Cast(start, size, ttl, angle, color);

    CastManager::get_instance()->add(cast);
}


void MageEnemy::remove_cast() {
    CastManager::get_instance()->remove(cast);

    delete cast;
    cast = nullptr;
}