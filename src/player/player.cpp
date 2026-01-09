#include "player.h"
#include "playerInput.h"

Player::Player()
    : position(0.0f), mousePosition(0.0f), speed(15.0f), size(1.0f, 1.0f) {
}

void Player::update(float dt, GLFWwindow* window) {
    PlayerInput::move(window, position, speed, dt);
    PlayerInput::updateMouse(window, mousePosition);
}