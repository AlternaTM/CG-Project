#include "player.h"
#include "playerInput.h"



PlayingState* PlayingState::instance() {
    static PlayingState inst;
    return &inst;
}

void PlayingState::enter(Player& player) {player.state = State::InGame;}
void PlayingState::update(Player& player, float dt, GLFWwindow* window) {
    bool moved = PlayerInput::move(window, player, player.speed, dt);

    PlayerInput::updateMouse(window, player.camera.getCameraPosition(), player.position, player.aimPosition, player.aimRotation);
    
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






