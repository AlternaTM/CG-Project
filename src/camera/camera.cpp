#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera(float screenWidth, float screenHeight)
    : position(0.0f),
    screenW(screenWidth),
    screenH(screenHeight),
    worldW(screenWidth),
    worldH(screenHeight) {
}

void Camera::setWorldBounds(float worldWidth, float worldHeight) {
    worldW = worldWidth;
    worldH = worldHeight;
}

void Camera::follow(const glm::vec2& target) {
    float halfW = screenW * 0.5f;
    float halfH = screenH * 0.5f;

    position.x = glm::clamp(
        target.x,
        -worldW * 0.5f + halfW,
        worldW * 0.5f - halfW
    );

    position.y = glm::clamp(
        target.y,
        -worldH * 0.5f + halfH,
        worldH * 0.5f - halfH
    );
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::translate(glm::mat4(1.0f),
        glm::vec3(-position, 0.0f));
}

glm::vec2 Camera::getCameraPosition()
{
    return position;
}