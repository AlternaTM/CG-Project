#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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

void PlayerInput::updateMouse(GLFWwindow* window, glm::vec3& mousePos)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int wW, wH;
    glfwGetWindowSize(window, &wW, &wH);

    mousePos.x = (float(mx) / wW) * 16.0f - 8.0f;
    mousePos.y = -((float(my) / wH) * 9.0f - 4.5f);
}
void PlayerInput::interact(GLFWwindow* window,Player& pl,std::vector<Chest>& chests) {
    static bool wasPressed = false;

    bool isPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    if (isPressed && !wasPressed)
    {
        for (int i = chests.size() - 1; i >= 0; i--) {  
            if (CollisionChecker::check_collision(pl, chests[i])) {
                chests.erase(chests.begin() + i);
            }
        }
    }

    wasPressed = isPressed;
}