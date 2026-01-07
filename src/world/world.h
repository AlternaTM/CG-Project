#pragma once

class World {
public:
    World(float worldWidth, float worldHeight);

    float getWidth() const { return width; }
    float getHeight() const { return height; }

private:
    float width;
    float height;
};