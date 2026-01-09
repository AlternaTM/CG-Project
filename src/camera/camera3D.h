#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera3D {

public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    Camera3D(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3 tgt = glm::vec3(0.0f, 0.0f, 0.0f))
        : position(pos), target(tgt), up(glm::vec3(0.0f, 1.0f, 0.0f)) {
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, up);
    }

    void setPosition(glm::vec3 pos) { position = pos; }
    void setTarget(glm::vec3 tgt) { target = tgt; }

    glm::vec3 getPosition() const { return position; }
};