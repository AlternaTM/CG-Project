#include "player.h"
#include "playerInput.h"
#include "shooting/bullet.h"


PlayingState* PlayingState::instance() {
    static PlayingState inst;
    return &inst;
}

void PlayingState::enter(Player& player) {player.state = State::InGame;}

void PlayingState::update(Player& player, float dt, GLFWwindow* window) {
    moved = PlayerInput::move(window, player, player.speed, dt);

    PlayerInput::updateMouse(window, player.camera.getCameraPosition(), *player.get_pos(), player.aimPosition, player.aimRotation);
    
    if(moved)
        update_anim(dt);

    if (player.shoot_timer > 0.0f) {
        player.shoot_timer -= dt;
    }
    if (PlayerInput::isMouseKeyPressed(window, GLFW_MOUSE_BUTTON_LEFT)) {
        player.shoot(dt);
    }
}

glm::vec2 PlayingState::get_offset() {
    if(moved)
        return Animable::get_offset();
    return { 0.0f,0.0f };
}

void PlayingState::hit(uint8_t* pl, uint8_t damage) {
    //std::cout << "Damage given" << std::endl;
    *pl -= damage;
}


//DeadState ---------------------------------

DeadState* DeadState::instance() {
    static DeadState inst;
    return &inst;
}

void DeadState::enter(Player& player) {

}

void DeadState::update(Player& player, float dt, GLFWwindow* window) {

}

//Player ---------------------------------

Player::Player(Camera& camera)
    :  aimPosition(0.0f), speed(15.0f), aimRotation(0.0f), camera(camera){
    currentState = PlayingState::instance();
    state = State::InGame;
    PlayerInput::register_input(GLFW_KEY_E);
    
    currentState->enter(*this);
}

void Player::update(float dt, GLFWwindow* window) {
    currentState->update(*this, dt, window);


}
void Player::set_state(PlayerState* state) {
    currentState->exit(*this);
    currentState = state;
    currentState->enter(*this);
}


glm::vec2 Player::get_offset() {
    return currentState->get_offset();
    //return { actual_frame * (1.0f / total_frame) ,0.0f };
}


glm::vec2 Player::get_frame_size() {
    return currentState->get_frame_size()                                                                                                                                                                                                                                                                               ;
}

void Player::hit(uint8_t damage) {
    currentState->hit(&life,damage);
}

void Player::shoot(float dt) {

    if (shoot_timer > 0.0f) {
        return;
    }
    shoot_timer = shoot_delay;

    glm::vec3 bulletDirection = glm::vec3(std::cos(aimRotation), std::sin(aimRotation), 0.0f);

    Bullet* bullet = new Bullet(bulletDirection,50,1.0f);
    bullet->set_position(pos);
    BulletManager::add_bullet(bullet);
}
