#include "button.h"
#include "player/playerInput.h"

Button::Button(
    const std::string& label,
    glm::vec2 position,
    glm::vec2 size,
	SpriteTexture texture,
    std::function<void()> onClick
)
    : label(label), position(position), size(size), button(texture), onClick(onClick), hasIcon(false) {
}

Button::Button(
    SpriteTexture icon,
    glm::vec2 iconSize,
    glm::vec2 position,
    glm::vec2 size,
    SpriteTexture texture,
    std::function<void()> onClick
)
    : icon(icon), iconSize(iconSize), position(position), size(size), button(texture), onClick(onClick), hasIcon(true) {
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
    spriteRenderer.Draw(
        button,
        position,
        size,
        0.0f,
        glm::mat4(1.0f),             
        { 0.0f, 0.0f },              
        { 1.0f, 1.0f },
        hovered ? hoverColor : normalColor
    );

    

    if (!hasIcon) {
        float textWidth = textRenderer.GetTextWidth(label, textScale);

        float textX = position.x - textWidth * 0.5f;
        float textY = position.y - textScale * 0.2f;

        textRenderer.RenderText(label, textX, textY, textScale, textColor);
	} else {
        spriteRenderer.Draw(
            icon,
            position,
            iconSize,
            0.0f,
            glm::mat4(1.0f),
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f }
        );
    }
}