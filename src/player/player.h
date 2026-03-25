#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

#include "camera/camera.h"
#include "playerinput.h"

class Player;

enum class State {
    InGame,
    Looting
};

class PlayerState {
public:
    virtual ~PlayerState() = default;

    virtual void enter(Player& player) {}
    virtual void update(Player& player, float dt, GLFWwindow* window) = 0;
    virtual void exit(Player& player) {}
};

class PlayingState : public PlayerState {
public:
    static PlayingState* instance();
    void enter(Player& player) override;
    void update(Player& player, float dt, GLFWwindow* window) override;
};


class LootingState : public PlayerState {
public:
    static LootingState* instance();
    void enter(Player& player) override;
    void update(Player& player, float dt, GLFWwindow* window) override;
};



class Player {
public:
    State state;

    glm::vec3 position;
    float speed;
    glm::vec2 size;
    Camera& camera;
    glm::vec3 aimPosition;
    float aimRotation;

    Player(Camera& camera);

    void update(float dt, GLFWwindow* window);
    void set_state(PlayerState* state);
    PlayerState* currentState;
   
};



