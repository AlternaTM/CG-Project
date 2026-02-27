#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>

#include "camera/camera.h"
class Player;

class PlayerState {
public:
    virtual ~PlayerState() = default;

    virtual void enter(Player& player) {}
    virtual void update(Player& player, float dt, GLFWwindow* window) = 0;
    virtual void exit(Player& player) {}
};



class Player {
public:
    enum class STATUS {
        PLAYING,
        REWARD
    };

    STATUS status;
    glm::vec3 position;
    float speed;
    glm::vec2 size;
    Camera& camera;
    glm::vec3 aimPosition;
    float aimRotation;



    Player(Camera& camera);

    void update(float dt, GLFWwindow* window);

private:
    class RunningState;
    class IdleState;
    class LootingState;
    class MenuState;


    class RunningState : public PlayerState {
    public:
        void update(Player& player, float dt, GLFWwindow* window) {
            bool moved = PlayerInput::move(window, player, player.speed, dt);

            PlayerInput::updateMouse(window, player.camera.getCameraPosition(), player.position, player.aimPosition, player.aimRotation);
            if (moved == false)
                player.currentState = &player.idleState;
        }
    };
    class LootingState : public PlayerState {
    public:
        void update(Player& player, float dt, GLFWwindow* window) {
            static bool wasPressed = false;
            bool isPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;


            if (isPressed && !wasPressed)
            {
                player.status = Player::STATUS::PLAYING;
            }

            wasPressed = isPressed;
        }
    };

    class IdleState : public PlayerState {
    public:
        void update(Player& player, float dt, GLFWwindow* window) {
            bool moved = PlayerInput::move(window, player, player.speed, dt);

            PlayerInput::updateMouse(window, player.camera.getCameraPosition(), player.position, player.aimPosition, player.aimRotation);
            if (moved == true)
                player.currentState = &player.runningState;
        }
    };

    class MenuState : public PlayerState {
    public:
        void update(Player& player, float dt, GLFWwindow* window) {
            static bool wasPressed = false;
            bool isPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;


            if (isPressed && !wasPressed)
            {
                player.status = Player::STATUS::PLAYING;
            }

            wasPressed = isPressed;
        }
    };


    RunningState runningState;
    IdleState idleState; 
    LootingState lootingState;
    MenuState menuState;

    PlayerState* currentState;
};

