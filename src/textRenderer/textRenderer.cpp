#include "textRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

TextRenderer::TextRenderer()
    : textShader("shaders/glsl/textVertexShader.glsl", "shaders/glsl/textFragShader.glsl")
{
    glm::mat4 projection = glm::ortho(
        -8.0f, 8.0f,
        -4.5f, 4.5f
    );

    textShader.use();
    textShader.setMat4("uProjection", projection);
    textShader.setInt("uText", 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TextRenderer::LoadFont(const std::string& fontPath, unsigned int fontSize)
{
    Characters.clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType\n";
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font\n";
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
         glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
         static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(
    const std::string& text,
    float x,
    float y,
    float scale,
    const glm::vec3& color
) {
    textShader.use();
    textShader.setVec3("uTextColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float worldScale = scale / PIXELS_PER_WORLD_UNIT;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        const Character& ch = Characters[*c];

        float xpos = x + ch.Bearing.x * worldScale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * worldScale;

        float w = ch.Size.x * worldScale;
        float h = ch.Size.y * worldScale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * worldScale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


float TextRenderer::GetTextWidth(const std::string& text, float scale) const
{
    float worldScale = scale / PIXELS_PER_WORLD_UNIT;
    float width = 0.0f;
    for (char c : text) {
        auto it = Characters.find(c);
        if (it == Characters.end()) continue;
        width += (it->second.Advance >> 6) * worldScale;
    }
    return width;
}