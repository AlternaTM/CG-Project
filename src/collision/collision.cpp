#include "collision.h"
#include <iostream>

bool CollisionChecker::check_collision(Entity& e1, Entity& e2)
{
    glm::vec2 e1_size = *(e1.get_size());
    glm::vec2 e2_size = *(e2.get_size());

    bool collisionX =
        std::abs(e1.get_pos()->x - e2.get_pos()->x) <=
        (e1_size.x * 0.5f + e2_size.x * 0.5f);

    bool collisionY =
        std::abs(e1.get_pos()->y - e2.get_pos()->y) <=
        (e1_size.y * 0.5f + e2_size.y * 0.5f);

    return collisionX && collisionY;
}


float CollisionChecker::distance(Entity& e1, Entity& e2) {
    glm::vec2 e1_pos = *e1.get_pos();
    glm::vec2 e2_pos = *e2.get_pos();

    float x = e2_pos.x - e1_pos.x;
    float y =  e2_pos.y - e1_pos.y;

    return  std::sqrt((x * x) + (y * y));
}
