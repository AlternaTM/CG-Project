#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

#include "camera/camera.h"
#include "playerinput.h"
#include "animSystem/animSystem.h"
#include "entity.h"
#include "upgradeUI/upgrades.h"


class Player;

enum class State {
    InGame,
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
    float shoot_delay = 0.2f;
    float shoot_timer = shoot_delay;
    uint8_t base_damage = 20;
    //MIGLIORAMENTI 
    float damage_mult = 1.0f;
    float shot_speed_mult = 1.0f;
    float bullet_speed_mult = 1.0f;
    float bullet_sise_mult = 1.0f;
    uint8_t bullet_max_hit = 1;

    const float max_mult = 5.0f;
    const uint8_t max_hits = 5;

    Player(Camera& camera);

    bool moved = false;

    void update(float dt, GLFWwindow* window);
    void set_state(PlayerState* state);
    PlayerState* currentState;

    void shoot(float dt);

    uint8_t get_life();

    glm::vec2 get_offset() override;
    glm::vec2 get_frame_size() override;

    void hit(uint8_t damage);

    void apply_upgrade(Upgrades upgrade);


    void reset();
protected:
    uint8_t life = 255;
};



