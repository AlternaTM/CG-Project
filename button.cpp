#include "button.h"
#include "player/playerInput.h"

Button::Button(
    const std::string& label,
    glm::vec2 position,
    glm::vec2 size,
    std::function<void()> onClick
)
    : label(label), position(position), size(size), onClick(onClick) {
}

void Button::update(GLFWwindow* window) {
    glm::vec2 mouse = PlayerInput::getMousePos(window);

    glm::vec2 halfSize = size * 0.5f;
    hovered =
        mouse.x >= position.x - halfSize.x && mouse.x <= position.x + halfSize.x &&
        mouse.y >= position.y - halfSize.y && mouse.y <= position.y + halfSize.y;

    if (hovered && PlayerInput::isMouseButtonJustPressed(window, GLFW_MOUSE_BUTTON_LEFT)) {
        if (onClick) onClick();
    }
}

void Button::render(SpriteRenderer& spriteRenderer, TextRenderer& textRenderer) {
    spriteRenderer.DrawColor(
        position,
        size,
        hovered ? hoverColor : normalColor
    );

    float textWidth = textRenderer.GetTextWidth(label, textScale);

    float textX = position.x - textWidth * 0.5f;
    float textY = position.y - (textScale / 120.0f) * 0.65f; // piccolo aggiustamento per centratura verticale

    textRenderer.RenderText(label, textX, textY, textScale, textColor);
}