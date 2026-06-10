#include "astroEnemy.h"


#include "mageBullet/mageCast.h"
#include <glm/gtc/matrix_transform.hpp>

// ============ RangedAttackState

void AstroAttackState::enter(Enemy& e) {
    owner = &e;
    reset_anim();
    AstroEnemy* me = dynamic_cast<AstroEnemy*>(&e);
    if (me->saved_target.x < e.get_pos()->x) {
        flipped = true;
    }
    else {
        flipped = false;
    }




}

void AstroAttackState::update(Enemy& e, float dt) {
    AstroEnemy* me = dynamic_cast<AstroEnemy*>(&e);
    update_anim(dt);
}

void AstroAttackState::exit(Enemy& e) {
    AstroEnemy* me = dynamic_cast<AstroEnemy*>(&e);
    me->remove_cast();
    owner = nullptr;
}


void AstroAttackState::on_animation_end() {
    if (owner)
        owner->on_target_out_of_range();
}



// ============ RangedAttackState

void AstroPreAttackState::enter(Enemy& e) {
    WaitingState::enter(e);
    AstroEnemy* em = dynamic_cast<AstroEnemy*>(&e);
    em->saved_target = *EnemyManager::_PLAYER->get_pos();





    glm::vec2 start = glm::vec2(em->saved_target.x, em->saved_target.y);
    glm::vec2 size = glm::vec2(3.0f, 3.0f);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(start, 0.0f));
    //model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    std::cout << "spawn" << std::endl;

    em->spawn_cast(
        model,
        glm::vec4(1.0f, 0.0f, 0.0f, 0.3f)
    );

}
void AstroPreAttackState::exit(Enemy& e) {
    WaitingState::exit(e);
    AstroEnemy* em = dynamic_cast<AstroEnemy*>(&e);
    em->remove_cast();
}


// ====================================

AstroEnemy::AstroEnemy()
    :preAttackState(2.0f, &attackState), waitingForEnemy(1.0f, &preAttackState), cast(nullptr)
{
    init_states();
    life = 255;
}

void AstroEnemy::init_states() {
    attackState.tot_framex = 8;
    attackState.tot_rows = 4;
    attackState.frame_duration = 0.15f;
    attackState.y_offset = 2;
    attackState.max_frame = 8;


    preAttackState.tot_framex = 8;
    preAttackState.tot_rows = 4;
    preAttackState.frame_duration = 0.15f;
    preAttackState.y_offset = 3;
    preAttackState.max_frame = 8;
    preAttackState.follow_player_dir_at_start = true;


    waitingForEnemy.tot_framex = 8;
    waitingForEnemy.tot_rows = 4;
    waitingForEnemy.frame_duration = 0.15f;
    waitingForEnemy.y_offset = 3;
    waitingForEnemy.max_frame = 8;
    waitingForEnemy.follow_player_dir_at_start = true;

    currentState = &waitingForEnemy;

    size.x = 2.0f;
    size.y = 2.0f;
}

void AstroEnemy::on_target_in_range() { change_state(&preAttackState); }
void AstroEnemy::on_target_out_of_range() { change_state(&waitingForEnemy); }



void AstroEnemy::spawn_cast(glm::mat4 model, const glm::vec4& color) {

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


    cast = new Cast(mesh, model, CASTTYPE::AstroCast, color);

    CastManager::get_instance()->add(cast);
}


void AstroEnemy::remove_cast() {
    CastManager::get_instance()->remove(cast);

    delete cast;
    cast = nullptr;
}