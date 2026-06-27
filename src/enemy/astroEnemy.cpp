#include "astroEnemy.h"
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include "collision/collision.h"

// ============ RangedAttackState

void AstroAttackState::enter(Enemy& e) {
    elapsed = 0.0f;
    owner = &e;
    reset_anim();
    AstroEnemy* em = dynamic_cast<AstroEnemy*>(&e);
    if (em->saved_target.x < e.get_pos()->x) {
        flipped = true;
    }
    else {
        flipped = false;
    }

    glm::vec2 start = glm::vec2(em->saved_target.x, em->saved_target.y);
    glm::vec2 size = glm::vec2(3.0f, 2.2f);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(start, 0.0f));
    //model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));


    em->spawn_cast(
        model,
        AstroShadowCast,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.4f)
    );




    // ASTEROIDE 3D 

    Asteroid* asteroid = em->spawn_asteroid();
    asteroid->set_scale(glm::vec3(2.0f));
    asteroid->get_pos()->x = em->saved_target.x;
    asteroid->get_pos()->y = em->saved_target.y + 8.0f;
    asteroid->get_pos()->z = -3.0f;

    asteroid->rotation_speed = glm::vec3(45.0f);

}

void AstroAttackState::update(Enemy& e, float dt) {
    AstroEnemy* me = dynamic_cast<AstroEnemy*>(&e);
    elapsed += dt;

    if (elapsed > 4.0f) {
        elapsed = 0.0f;
        Entity e;
        e.get_pos()->x = me->saved_target.x;
        e.get_pos()->y = me->saved_target.y;

        if (CollisionChecker::check_collision(*EnemyManager::_PLAYER, e)) {
            EnemyManager::_PLAYER->hit(me->get_base_damage());
        }

        me->on_target_out_of_range();
        return;
    }

    Asteroid* asteroids = me->get_asteroid();

    float t = glm::clamp(elapsed / 4.0f, 0.0f, 1.0f);
    float perc = glm::mix(me->saved_target.y + 8.0f, me->saved_target.y - 0.5f, t);

    float current_scale = glm::mix(2.0f, 1.1f, t);
    asteroids->set_scale(glm::vec3(current_scale));

    asteroids->set_rotation(asteroids->get_rotation() + asteroids->rotation_speed * dt);
    asteroids->get_pos()->y = perc;

   

    update_anim(dt);








}

void AstroAttackState::exit(Enemy& e) {
    AstroEnemy* me = dynamic_cast<AstroEnemy*>(&e);
    me->remove_cast();
    me->remove_asteroid();
    owner = nullptr;
}


void AstroAttackState::on_animation_end() {
    //if (owner)
    //    owner->on_target_out_of_range();
}



// ============ RangedAttackState

void AstroPreAttackState::enter(Enemy& e) {
    WaitingState::enter(e);
    AstroEnemy* em = dynamic_cast<AstroEnemy*>(&e);
    em->saved_target = *EnemyManager::_PLAYER->get_pos();





    glm::vec2 start = glm::vec2(em->saved_target.x, em->saved_target.y);
    glm::vec2 size = glm::vec2(3.0f, 2.2f);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(start, 0.0f));
    //model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));


    em->spawn_cast(
        model,
        CASTTYPE::AstroCast,
        glm::vec4(1.0f, 0.0f, 0.0f, 0.3f)
    );

}
void AstroPreAttackState::exit(Enemy& e) {
    WaitingState::exit(e);
    AstroEnemy* em = dynamic_cast<AstroEnemy*>(&e);
    em->remove_cast();
}


// ========================




// ====================================

AstroEnemy::AstroEnemy()
    :preAttackState(2.0f, &attackState), waitingForEnemy(randomWaitTime(), &preAttackState), cast(nullptr), Enemy(EnemyTipe::Astro)
{
    init_states();
    size = glm::vec2(0.7f,0.7f);
    life = 255;
}

void AstroEnemy::init_states() {
    attackState.tot_framex = 6;
    attackState.tot_rows = 1;
    attackState.frame_duration = 0.40f;
    attackState.y_offset = 0;
    attackState.max_frame = 2;


    preAttackState.tot_framex = 6;
    preAttackState.tot_rows = 1;
    preAttackState.frame_duration = 0.33f;
    preAttackState.y_offset = 0;
    preAttackState.max_frame = 6;
    preAttackState.follow_player_dir_at_start = false;


    waitingForEnemy.tot_framex = 6;
    waitingForEnemy.tot_rows = 1;
    waitingForEnemy.frame_duration = 3.0f;
    waitingForEnemy.y_offset = 0;
    waitingForEnemy.max_frame = 1;
    waitingForEnemy.follow_player_dir_at_start = true;

    currentState = &waitingForEnemy;

    size.x = 2.0f;
    size.y = 2.0f;
}

void AstroEnemy::on_target_in_range() { change_state(&preAttackState); }
void AstroEnemy::on_target_out_of_range() { change_state(&waitingForEnemy); }



void AstroEnemy::spawn_cast(glm::mat4 model, const CASTTYPE type, const glm::vec4& color) {

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


    cast = new Cast(mesh, model, type, color);
    cast->startTime = (float)glfwGetTime();

    CastManager::get_instance()->add(cast);
}


void AstroEnemy::remove_cast() {
    if (cast) {
        CastManager::get_instance()->remove(cast);
        delete cast;
        cast = nullptr;
    }
   


}

void AstroEnemy::remove_asteroid() {

    if (asteroids != nullptr) {
        CastManager::get_instance()->remove_aster(asteroids);
        delete asteroids;
        asteroids = nullptr;
    }


}


Asteroid* AstroEnemy::spawn_asteroid() {
    remove_asteroid();
    asteroids = new Asteroid();
    CastManager::get_instance()->add_aster(asteroids);
    return asteroids;
}


Asteroid* AstroEnemy::get_asteroid() {
    return asteroids;
}

float AstroEnemy::randomWaitTime() {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(1.0f, 4.5f);
    return dist(rng);
}