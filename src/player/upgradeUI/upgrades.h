#pragma once

#include <glm/glm.hpp>
#include <string>
#include "spriteRenderer/spriteRenderer.h"
#include "camera/camera.h"
#include "figureRenderer/figureRenderer.h"
#include <glm/glm.hpp>

#include "animSystem/animSystem.h"
#include "entity.h"
#include <vector>

class FigRenderer;
enum Upgrades {
	BulletSpeed,
	BulletSize,
	ShootingSpeed,
	BulletDamager
};


class Card {

public:
	Upgrades upgrade;
	std::string text;
	glm::vec2 size;
	glm::vec2 pos;

	Card(glm::vec2 pos, glm::vec2 size) : size(size),pos(pos){
	}

};


class UpgradeUI {
	Card* card1 = nullptr;
	Card* card2 = nullptr;
	Card* card3 = nullptr;

	
public:
	bool shouldRender = false;
	UpgradeUI() {

	}
	~UpgradeUI() {
		destroyScene();
	}
	void update(float dt);
	void render(FigRenderer* renderer, Camera& camera);
	void createScene();
	void destroyScene();
};