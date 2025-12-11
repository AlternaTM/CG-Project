#include <stb/stb_image.h>
#include <iostream>

#include <world/world.h>

World::World(const std::string& bgTexturePath, float worldWidth, float worldHeight)
    : width(worldWidth), height(worldHeight)
{
    // Load texture
    int texWidth, texHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(bgTexturePath.c_str(), &texWidth, &texHeight, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load background texture: " << bgTexturePath << std::endl;
        return;
    }

    glGenTextures(1, &backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (nrChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Setup quad
    setupQuad();
}

World::~World() {
    glDeleteVertexArrays(1, &bgVAO);
    glDeleteBuffers(1, &bgVBO);
    glDeleteBuffers(1, &bgEBO);
    glDeleteTextures(1, &backgroundTexture);
}

void World::setupQuad() {
    float vertices[] = {
        // position             // texture
        0.0f, 0.0f, 0.0f,       0.0f, 0.0f,
        width, 0.0f, 0.0f,      1.0f, 0.0f,
        width, height, 0.0f,    1.0f, 1.0f,
        0.0f, height, 0.0f,     0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);
    glGenBuffers(1, &bgEBO);

    glBindVertexArray(bgVAO);

    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void World::draw(Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("uModel", model);

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glBindVertexArray(bgVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
