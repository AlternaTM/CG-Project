#include "MageEnemy.h"


#include "mageBullet/mageCast.h"
#include <glm/gtc/matrix_transform.hpp>

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
    start_time = (float)glfwGetTime();
    ttl = 2.0f;
    glm::vec2 start = glm::vec2(e.get_pos()->x, e.get_pos()->y);
    glm::vec2 size = glm::vec2(20.0f, 0.5f);
    glm::vec2 dir = glm::normalize(me->saved_target - start);
    float angle = atan2(dir.y, dir.x);


    float elapsed = 0.0f;
    float maxLength = size.x; //8.0f;
    float speed = 12.0f;
    float beamWidth = size.y;//0.28f;


    float currentLength = glm::min(elapsed * speed, maxLength);

    glm::vec2 forward = glm::vec2(cos(angle), sin(angle));
    glm::vec2 beamCenter = start + forward * (currentLength * 0.5f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(beamCenter, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(currentLength, beamWidth, 1.0f));


    me->spawn_cast(
        model,
        glm::vec4(0.7f, 0.0f, 0.3f, 0.97f)
    );


    attackSound = EnemyManager::get_instance()->playCostantSound("assets/audio/laser.wav");

}

void RangedAttackState::update(Enemy& e,float dt) {
    MageEnemy* me = dynamic_cast<MageEnemy*>(&e);
    ttl -= dt;

    glm::vec2 start = glm::vec2(e.get_pos()->x, e.get_pos()->y);
    glm::vec2 size = glm::vec2(20.0f, 0.5f);
    glm::vec2 dir = glm::normalize(me->saved_target - start);
    float angle = atan2(dir.y, dir.x);


    float elapsed = start_time - ttl;
    float maxLength = size.x; //8.0f;
    float speed = 12.0f;
    float beamWidth = size.y;//0.28f;


    float currentLength = glm::min(elapsed * speed, maxLength);

    glm::vec2 forward = glm::vec2(cos(angle), sin(angle));
    glm::vec2 beamCenter = start + forward * (currentLength * 0.5f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(beamCenter, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(currentLength, beamWidth, 1.0f));

    me->get_cast()->update_model(model);

    
    if (checkBeamHit(*EnemyManager::_PLAYER->get_pos(), *EnemyManager::_PLAYER->get_size(),
        start, forward, beamWidth , currentLength)) {
        EnemyManager::_PLAYER->hit(e.get_base_damage());
    }

    update_anim(dt);
}

void RangedAttackState::exit(Enemy& e) {
    MageEnemy* me = dynamic_cast<MageEnemy*>(&e);
    me->remove_cast();
    EnemyManager::get_instance()->stopConstantSound(attackSound);
    attackSound = nullptr;
    owner = nullptr;
}


void RangedAttackState::on_animation_end() {
    if(owner)
        owner->on_target_out_of_range();
}


bool RangedAttackState::checkBeamHit(const glm::vec2& objPos, const glm::vec2& objSize,
    const glm::vec2 start, const glm::vec2 forward,
    const float beamWidth, const float currentLength) const {
    glm::vec2 right(-forward.y, forward.x);

    glm::vec2 beamCenter = start + forward * (currentLength * 0.5f);
    glm::vec2 beamHalf(currentLength * 0.5f, beamWidth * 0.5f);
    glm::vec2 boxHalf = (objSize * 0.2f) * 0.5f;;

    glm::vec2 d = beamCenter - objPos;

    glm::vec2 axes[4] = { forward, right, glm::vec2(1,0), glm::vec2(0,1) };

    for (const glm::vec2& axis : axes) {
        float beamProj = beamHalf.x * fabs(glm::dot(forward, axis))
            + beamHalf.y * fabs(glm::dot(right, axis));
        float boxProj = boxHalf.x * fabs(axis.x) + boxHalf.y * fabs(axis.y);
        float dist = fabs(glm::dot(d, axis));

        if (dist > beamProj + boxProj)
            return false;
    }

    return true;
}


// ============ RangedAttackState

void PreAttackState::enter(Enemy& e) {
    WaitingState::enter(e);
    MageEnemy* em = dynamic_cast<MageEnemy*>(&e);
    em->saved_target = *EnemyManager::_PLAYER->get_pos();

    start_time = (float)glfwGetTime();

    glm::vec2 start = glm::vec2(e.get_pos()->x, e.get_pos()->y);
    glm::vec2 size = glm::vec2(20.0f, 0.5f);
    glm::vec2 dir = glm::normalize(em->saved_target - start);
    float angle = atan2(dir.y, dir.x);


    float elapsed = 0.0f;
    float maxLength = size.x; //8.0f;
    float speed = 12.0f;
    float beamWidth = size.y;//0.28f;


    float currentLength = glm::min(elapsed * speed, maxLength);

    glm::vec2 forward = glm::vec2(cos(angle), sin(angle));
    glm::vec2 beamCenter = start + forward * (currentLength * 0.5f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(beamCenter, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(currentLength, beamWidth, 1.0f));


    em->spawn_cast(
        model,
        glm::vec4(1.0f, 0.0f, 0.0f, 0.3f)
    );
}

void PreAttackState::update(Enemy& e,float dt) {
    WaitingState::update(e, dt);
    MageEnemy* em = dynamic_cast<MageEnemy*>(&e);
    glm::vec2 start = glm::vec2(e.get_pos()->x, e.get_pos()->y);
    glm::vec2 size = glm::vec2(20.0f, 0.5f);
    glm::vec2 dir = glm::normalize(em->saved_target - start);
    float angle = atan2(dir.y, dir.x);


    float elapsed = (float)glfwGetTime() - start_time;
    float maxLength = size.x; //8.0f;
    float speed = 12.0f;
    float beamWidth = size.y;//0.28f;


    float currentLength = glm::min(elapsed * speed, maxLength);

    glm::vec2 forward = glm::vec2(cos(angle), sin(angle));
    glm::vec2 beamCenter = start + forward * (currentLength * 0.5f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(beamCenter, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(currentLength, beamWidth, 1.0f));

    em->get_cast()->update_model(model);
}


void PreAttackState::exit(Enemy& e) {
    WaitingState::exit(e);
    MageEnemy* em = dynamic_cast<MageEnemy*>(&e);
    em->remove_cast();
}


// ====================================

MageEnemy::MageEnemy() 
    :preAttackState(2.0f,&attackState), endAttackDelay(1.0f, &movingState), cast(nullptr), Enemy(EnemyTipe::Mage)
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



void MageEnemy::spawn_cast(glm::mat4 model, const glm::vec4& color) {

    remove_cast();

    Mesh2D mesh;
    std::vector<Vertex2D> vertices = {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f}, {0.0f, 1.0f}},
    };
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    mesh.create(vertices, indices);


    cast = new Cast(mesh, model, CASTTYPE::MageCast, color);

    CastManager::get_instance()->add(cast);
}


void MageEnemy::remove_cast() {
    CastManager::get_instance()->remove(cast);

    delete cast;
    cast = nullptr;
}


irrklang::ISound* MageEnemy::get_sound() {
    return attackState.attackSound;
}