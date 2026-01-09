#include "collision.h"
bool CollisionChecker::check_collision(const Player& pl, const Chest& chest)
{
    glm::vec2 player_size = pl.size;
    glm::vec2 chest_size = chest.size;

    bool collisionX =
        std::abs(pl.position.x - chest.position.x) <=
        (player_size.x * 0.5f + chest_size.x * 0.5f);

    bool collisionY =
        std::abs(pl.position.y - chest.position.y) <=
        (player_size.y * 0.5f + chest_size.y * 0.5f);

    return collisionX && collisionY;
}