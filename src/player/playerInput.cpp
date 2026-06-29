
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
        return glfwGetKey(window, l) == GLFW_PRESS;
    }
    return  (!it->second && glfwGetKey(window, l));
}

void PlayerInput::update_input(GLFWwindow* window) {//questo va chiamato DOPO che si sono fatti tutti i controlli, perché aggiorna il valore passato
    for (auto& pair : wasPressed) {
        pair.second = glfwGetKey(window, pair.first) == GLFW_PRESS;
    }
}

glm::vec2 PlayerInput::move(GLFWwindow* window, Player& pl, float speed, float dt)
{
    glm::vec3* pl_pos = pl.get_pos();
    glm::vec2 movedDir = glm::vec2(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W)) movedDir.y = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S)) movedDir.y = -1.0f;
    if (glfwGetKey(window, GLFW_KEY_A)) movedDir.x = -1.0f;
    if (glfwGetKey(window, GLFW_KEY_D)) movedDir.x = 1.0f;

    if (movedDir.x != 0.0f || movedDir.y != 0.0f) {
        glm::vec2 normalizedDir = glm::normalize(movedDir);
        pl_pos->x += normalizedDir.x * speed * dt;
        pl_pos->y += normalizedDir.y * speed * dt;

        pl_pos->x = glm::clamp(pl_pos->x, -15.5f, 15.5f);
        pl_pos->y = glm::clamp(pl_pos->y, -8.5f, 8.5f);
    }


    return movedDir;
}

glm::vec2 PlayerInput::getMousePos(GLFWwindow* window)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int wW, wH;
    glfwGetWindowSize(window, &wW, &wH);

    glm::vec2 mousePos;
    mousePos.x = (float(mx) / wW) * 16.0f - 8.0f;
    mousePos.y = -((float(my) / wH) * 9.0f - 4.5f);

	return mousePos;
}

void PlayerInput::updateMouse(GLFWwindow* window, glm::vec2 cameraPos, glm::vec3 playerPos, glm::vec3& aimPos, float& aimRotation)
{
    glm::vec2 mousePos = getMousePos(window);

    glm::vec2 playerViewPos = glm::vec2(playerPos.x, playerPos.y) - cameraPos;

    glm::vec2 dir = glm::normalize(glm::vec2(mousePos.x - playerViewPos.x, mousePos.y - playerViewPos.y));

    aimRotation = std::atan2(dir.y, dir.x);

    aimPos = playerPos + glm::vec3(dir * 1.0f, 0.0f);
}

void PlayerInput::interact(GLFWwindow* window,Player& pl) {

}

bool PlayerInput::isMouseKeyPressed(GLFWwindow* window, int l) {
    return glfwGetMouseButton(window, l) == GLFW_PRESS;
}

bool PlayerInput::isMouseButtonJustPressed(GLFWwindow* window, int l) {
    auto it = wasPressed.find(l);
    if (it == wasPressed.end()) {
        std::cout << "Tasto mouse " << l << " non registrato!" << std::endl;
        return false;
    }
    return (!it->second && glfwGetMouseButton(window, l) == GLFW_PRESS);
}
