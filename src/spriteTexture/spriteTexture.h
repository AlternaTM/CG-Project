#pragma once

#include <glad/glad.h>

class SpriteTexture {
public:
    unsigned int ID;
    int width, height;

    SpriteTexture();
    void generate(int width, int height, unsigned char* data);
    void bind() const;
};

