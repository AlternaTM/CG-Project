#include "player.h"
#include "playerInput.h"
#include "shooting/bullet.h"
#include <algorithm> 

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
    if (damage >= *pl) {
        *pl = 0;
    }
    else {
        *pl -= damage;
    }
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
    :  aimPosition(0.0f), speed(4.5f), aimRotation(0.0f), camera(camera){
    currentState = PlayingState::instance();
    state = State::InGame;
    PlayerInput::register_input(GLFW_KEY_E);
	PlayerInput::register_input(GLFW_KEY_ESCAPE);
	PlayerInput::register_input(GLFW_MOUSE_BUTTON_LEFT);
    
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
    shoot_timer = shoot_delay / shot_speed_mult;

    glm::vec3 bulletDirection = glm::vec3(std::cos(aimRotation), std::sin(aimRotation), 0.0f);

    Bullet* bullet = new Bullet(bulletDirection, static_cast<uint8_t>(glm::clamp(base_damage * damage_mult, 0.0f, 255.0f)),2.0f * bullet_speed_mult, bullet_max_hit, bullet_sise_mult);
    bullet->set_position(pos);
    BulletManager::add_bullet(bullet);
}

void Player::apply_upgrade(Upgrades upgrade) {
    constexpr float increment = 0.2f;

    switch (upgrade) {
    case Upgrades::BulletDamager:
        damage_mult = std::min(damage_mult + increment, max_mult);
        break;

    case Upgrades::ShootingSpeed:
        shot_speed_mult = std::min(shot_speed_mult + increment, max_mult);
        break;

    case Upgrades::BulletSpeed:
        bullet_speed_mult = std::min(bullet_speed_mult + increment, max_mult);
        break;

    case Upgrades::BulletSize:
        bullet_sise_mult = std::min(bullet_sise_mult + increment, max_mult);
        break;

    case Upgrades::BulletPierce:
        bullet_max_hit = static_cast<uint8_t>(std::min<int>(bullet_max_hit + 1, max_hits));
        break;

    default:
        break;
    }

} 

uint8_t Player::get_life() {
    return life;
}

void Player::reset() {
    life = 255;
    damage_mult = 1.0f;
    shot_speed_mult = 1.0f;
    bullet_speed_mult = 1.0f;
    bullet_sise_mult = 1.0f;
    bullet_max_hit = 1;

    pos = glm::vec3(0.0f,0.0f,0.0f);
}