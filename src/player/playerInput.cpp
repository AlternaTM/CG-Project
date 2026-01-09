#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "playerInput.h"

void PlayerInput::move(GLFWwindow* window, Player& pl, float speed, float dt)
{
    if (pl.status == Player::STATUS::PLAYING) {
        if (glfwGetKey(window, GLFW_KEY_W)) pl.position.y += speed * dt;
        if (glfwGetKey(window, GLFW_KEY_S)) pl.position.y -= speed * dt;
        if (glfwGetKey(window, GLFW_KEY_A)) pl.position.x -= speed * dt;
        if (glfwGetKey(window, GLFW_KEY_D)) pl.position.x += speed * dt;

        pl.position.x = glm::clamp(pl.position.x, -15.5f, 15.5f);
        pl.position.y = glm::clamp(pl.position.y, -8.5f, 8.5f);
    }

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

void PlayerInput::interact(GLFWwindow* window,Player& pl,std::vector<Chest>& chests) {
    static bool wasPressed = false;

    bool isPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    if (isPressed && !wasPressed)
    {
        if (pl.status == Player::STATUS::PLAYING) {

            for (size_t i = chests.size(); i-- > 0;) {
                if (CollisionChecker::check_collision(pl, chests[i])) {
                    chests.erase(chests.begin() + i);
                    pl.status = Player::STATUS::REWARD;
                }
            }
        }
        else if(pl.status == Player::STATUS::REWARD){
            pl.status = Player::STATUS::PLAYING;
        }

    }

    wasPressed = isPressed;
}