#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "playerInput.h"

void PlayerInput::move(GLFWwindow* window, glm::vec3& pos, float speed, float dt) 
{
    if (glfwGetKey(window, GLFW_KEY_W)) pos.y += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_S)) pos.y -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_A)) pos.x -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_D)) pos.x += speed * dt;

    pos.x = glm::clamp(pos.x, -15.5f, 15.5f);
    pos.y = glm::clamp(pos.y, -8.5f, 8.5f);
}

void PlayerInput::updateMouse(GLFWwindow* window, glm::vec2 cameraPos, glm::vec3 playerPos, glm::vec3& aimPos, float& aimRotation)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int wW, wH;
    glfwGetWindowSize(window, &wW, &wH);

    glm::vec2 mousePos;
    mousePos.x = (float(mx) / wW) * 16.0f - 8.0f;
    mousePos.y = -((float(my) / wH) * 9.0f - 4.5f);

    glm::vec2 playerViewPos = glm::vec2(playerPos.x, playerPos.y) - cameraPos;

    glm::vec2 dir = glm::normalize(glm::vec2(mousePos.x - playerViewPos.x, mousePos.y - playerViewPos.y));

    aimRotation = std::atan2(dir.y, dir.x);

    aimPos = playerPos + glm::vec3(dir * 1.0f, 0.0f);
}