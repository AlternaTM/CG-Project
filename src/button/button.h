#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>

#include "spriteRenderer/spriteRenderer.h"
#include "textRenderer/textRenderer.h"

class Button {
protected:
    float textScale = 0.8f;
public:
    Button(
        const std::string& label,
        glm::vec2 position,   // spazio (-8..8 / -4.5..4.5)
        glm::vec2 size,       
        std::function<void()> onClick
    );

    void update(GLFWwindow* window);
    void render(SpriteRenderer& spriteRenderer, TextRenderer& textRenderer);

private:
    std::string label;
    glm::vec2 position;
    glm::vec2 size;
    std::function<void()> onClick;

    bool hovered = false;

    glm::vec4 normalColor = { 0.2f, 0.2f, 0.25f, 0.9f };
    glm::vec4 hoverColor = { 0.35f, 0.35f, 0.45f, 0.95f };
    glm::vec3 textColor = { 1.0f, 1.0f, 1.0f };

};