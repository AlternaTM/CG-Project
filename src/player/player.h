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
    Looting,
    Dead
};

class PlayerState {
public:
    virtual ~PlayerState() = default;

    virtual void enter(Player& player) {}
    virtual void update(Player& player, float dt, GLFWwindow* window) = 0;
    virtual void exit(Player& player) {}
    virtual glm::vec2 get_offset() {
        return { 0.0f,0.0f };
    }
    virtual glm::vec2 get_size() {
        return { 0.5f,1.0f };
    }
};

class PlayingState : public PlayerState {
private:
    bool moved = false;
public:
    const uint8_t total_frame = 2;
    uint8_t actual_frame = 0;
    float timer = 0.0f;
    float frame_duration = 0.1f;


    static PlayingState* instance();
    void enter(Player& player) override;
    void update(Player& player, float dt, GLFWwindow* window) override;
    glm::vec2 get_offset() override;
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

    bool moved = false;

    void update(float dt, GLFWwindow* window);
    void set_state(PlayerState* state);
    PlayerState* currentState;

    glm::vec2 get_offset();
    glm::vec2 get_size();
   
};



