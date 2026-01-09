#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "shader/shader.h"

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    unsigned int Advance; // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
    TextRenderer(unsigned int width, unsigned int height);
    ~TextRenderer();

    void LoadFont(const std::string& fontPath, unsigned int fontSize);

    void RenderText(
        const std::string& text,
        float x,
        float y,
        float scale,
        const glm::vec3& color
    );

private:
    std::map<char, Character> Characters;
    Shader textShader;

    unsigned int VAO;
    unsigned int VBO;
};