#pragma once
#include "player/player.h"
#include "chest/chest.h"
#include <glm/glm.hpp>


class CollisionChecker
{
public:
	static bool check_collision(const Player& pl, const Chest& chest);
};

