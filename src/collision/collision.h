#pragma once
#include "entity.h"
#include "chest/chest.h"
#include "enemy/enemy.h"
#include <glm/glm.hpp>

class Player;
class Chest;

class CollisionChecker
{
public:
	static bool check_collision(Entity& e1, Entity& e2);
};

	