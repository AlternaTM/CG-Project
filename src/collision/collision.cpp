#include "collision.h"
#include <iostream>

bool CollisionChecker::check_collision(Entity& e1, Entity& e2)
{
    glm::vec2 player_size = *(e1.get_size());
    glm::vec2 chest_size = *(e2.get_size());

    bool collisionX =
        std::abs(e1.get_pos()->x - e2.get_pos()->x) <=
        (player_size.x * 0.5f + chest_size.x * 0.5f);

    bool collisionY =
        std::abs(e1.get_pos()->y - e2.get_pos()->y) <=
        (player_size.y * 0.5f + chest_size.y * 0.5f);



    std::cout << std::boolalpha << (collisionX && collisionY) << std::endl;
    return collisionX && collisionY;
}

