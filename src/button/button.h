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
        glm::vec2 position,
        glm::vec2 size,       
		SpriteTexture texture,
        std::function<void()> onClick
    );

    Button(
        SpriteTexture icon,
        glm::vec2 iconSize,
        glm::vec2 position,
        glm::vec2 size,
        SpriteTexture texture,
        std::function<void()> onClick
    );

    void update(GLFWwindow* window);
    void render(SpriteRenderer& spriteRenderer, TextRenderer& textRenderer);

private:
    std::string label;
    bool hasIcon = false;
    glm::vec2 position;
    glm::vec2 size;
    std::function<void()> onClick;
    SpriteTexture button;
	SpriteTexture icon;
	glm::vec2 iconSize;

    bool hovered = false;

    glm::vec4 normalColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4 hoverColor = { 0.55f, 0.35f, 0.35f, 1.0f };
    glm::vec3 textColor = { 1.0f, 1.0f, 1.0f };

};