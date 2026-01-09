#include "player.h"
#include "playerInput.h"


Player::Player(Camera& camera)
    : position(0.0f), aimPosition(0.0f), speed(15.0f), aimRotation(0.0f), camera(camera), size(1.0f, 1.0f) {

}

void Player::update(float dt, GLFWwindow* window) {
    PlayerInput::move(window, position, speed, dt);

    PlayerInput::updateMouse(window, camera.getCameraPosition(), position, aimPosition, aimRotation);
}