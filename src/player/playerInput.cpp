
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdio.h>
#include "playerInput.h"
#include "player.h"


std::unordered_map<int, bool> PlayerInput::wasPressed = {};

void PlayerInput::register_input(int l) {
    wasPressed.insert({ l, false });
}
bool PlayerInput::isPressed(GLFWwindow* window, int l) {
    return glfwGetKey(window, l);
}
bool PlayerInput::isKeyJustPressed(GLFWwindow* window, int l) {
    auto it = wasPressed.find(l);
    if (it == wasPressed.end()) {
        std::cout << "Tasto " << l << " non registrato!" << std::endl;
        return false;
    }
    return  (!it->second && glfwGetKey(window, l));
}
void PlayerInput::update_input(GLFWwindow* window) {//questo va chiamato DOPO che si sono fatti tutti i controlli, perché aggiorna il valore passato
    for (auto& pair : wasPressed) {
        pair.second = glfwGetKey(window, pair.first) == GLFW_PRESS;
    }
}

bool PlayerInput::move(GLFWwindow* window, Player& pl, float speed, float dt)
{
    bool moved = false;
    if (glfwGetKey(window, GLFW_KEY_W)) {
        pl.position.y += speed * dt;
    } 
    if (glfwGetKey(window, GLFW_KEY_S)) {
        pl.position.y -= speed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        pl.position.x -= speed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        pl.position.x += speed * dt;
    }
    if (moved) {
        pl.position.x = glm::clamp(pl.position.x, -15.5f, 15.5f);
        pl.position.y = glm::clamp(pl.position.y, -8.5f, 8.5f);
    }
    return moved;
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

void PlayerInput::interact(GLFWwindow* window,Player& pl) {

}
