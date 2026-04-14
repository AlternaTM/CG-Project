#include "player.h"
#include "playerInput.h"



PlayingState* PlayingState::instance() {
    static PlayingState inst;
    return &inst;
}

void PlayingState::enter(Player& player) {player.state = State::InGame;}

void PlayingState::update(Player& player, float dt, GLFWwindow* window) {
    moved = PlayerInput::move(window, player, player.speed, dt);

    PlayerInput::updateMouse(window, player.camera.getCameraPosition(), player.position, player.aimPosition, player.aimRotation);
    

    timer += dt;
    if (timer >= frame_duration) {
        timer = 0;
        actual_frame = (actual_frame + 1) % total_frame;
    }


}

glm::vec2 PlayingState::get_offset() {
    if(moved)
        return { actual_frame * (1.0f / total_frame) ,0.0f };
    return { 0.0f,0.0f };
}



LootingState* LootingState::instance() {
    static LootingState inst;
    return &inst;
}
void LootingState::enter(Player& player) {player.state = State::Looting;}
void LootingState::update(Player& player, float dt, GLFWwindow* window) {

    if (PlayerInput::isKeyJustPressed(window, GLFW_KEY_E)) {
        player.set_state(PlayingState::instance());
    }
}


//Player ---------------------------------

Player::Player(Camera& camera)
    : position(0.0f), aimPosition(0.0f), speed(15.0f), aimRotation(0.0f), camera(camera), size(1.0f, 1.0f) {
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


glm::vec2 Player::get_size() {
    return currentState->get_size()                                                                                                                                                                                                                                                                               ;
}




