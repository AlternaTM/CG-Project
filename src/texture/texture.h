#pragma once

#include <glad/glad.h>

class Texture {
public:
    unsigned int ID;
    int width, height;

    Texture();
    void generate(int width, int height, unsigned char* data);
    void bind() const;
};

