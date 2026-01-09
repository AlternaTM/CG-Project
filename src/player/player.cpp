#include "player.h"
#include "playerInput.h"

Player::Player(Camera& camera)
    : position(0.0f), mousePosition(0.0f), speed(15.0f), rotation(0.0f), camera(camera) {

}

void Player::update(float dt, GLFWwindow* window) {
    PlayerInput::move(window, position, speed, dt);

    PlayerInput::updateMouse(window, camera.getCameraPosition(), position, mousePosition, rotation);

    glm::vec2 diff = glm::vec2(mousePosition) - glm::vec2(position);
    rotation = std::atan2(diff.y, diff.x); // ritorna angolo in radianti
}