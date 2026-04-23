#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

#include "camera/camera.h"
#include "playerinput.h"
#include "animSystem/animSystem.h"
#include "entity.h"

class Player;

enum class State {
    InGame,
    Looting,
    Dead
};

class PlayerState : public Renderable{
public:
    virtual ~PlayerState() = default;

    virtual void enter(Player& player) {}
    virtual void update(Player& player, float dt, GLFWwindow* window) = 0;
    virtual void exit(Player& player) {}
    virtual glm::vec2 get_offset() override {
        return { 0.0f,0.0f };
    }
    virtual glm::vec2 get_frame_size() override {
        return { 0.5f,1.0f };
    }

    virtual void hit(uint8_t* player,uint8_t damage) {}

};

class PlayingState : public PlayerState, public Animable {
private:
    bool moved = false;
    PlayingState() {
        tot_framex = 2;
        max_frame = 2;
        frame_duration = 0.1f;

    }
public:
    static PlayingState* instance();
    void enter(Player& player) override;
    void update(Player& player, float dt, GLFWwindow* window) override;
    void on_animation_end() override {}

    glm::vec2 get_offset() override;

    void hit(uint8_t* player, uint8_t damage) override;
};


class LootingState : public PlayerState {
public:
    static LootingState* instance();
    void enter(Player& player) override;
    void update(Player& player, float dt, GLFWwindow* window) override;
};


class DeadState : public PlayerState {
public: 
    static DeadState* instance();
    void enter(Player& player) override;
    void update(Player& player, float dt, GLFWwindow* window) override;
};

class Player : Renderable, public Entity  {

public:
    State state;
    float speed;
    Camera& camera;
    glm::vec3 aimPosition;
    float aimRotation;

    Player(Camera& camera);

    bool moved = false;

    void update(float dt, GLFWwindow* window);
    void set_state(PlayerState* state);
    PlayerState* currentState;

    glm::vec2 get_offset() override;
    glm::vec2 get_frame_size() override;

    void hit(uint8_t damage);
protected:
    uint8_t life = 255;
};



