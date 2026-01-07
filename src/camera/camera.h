#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);

    void setWorldBounds(float worldWidth, float worldHeight);
    void follow(const glm::vec2& target);

    glm::mat4 getViewMatrix() const;

private:
    glm::vec2 position;
    float screenW, screenH;
    float worldW, worldH;
};