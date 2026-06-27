#pragma once

#include <glm/glm.hpp>
#include <string>
#include "spriteRenderer/spriteRenderer.h"
#include "camera/camera.h"
#include "figureRenderer/figureRenderer.h"
#include "resourceManager/resourceManager.h"
#include "textRenderer/textRenderer.h"
#include "animSystem/animSystem.h"
#include "entity.h"
#include "button/button.h"
#include <vector>
#include <random>


class FigRenderer;
enum Upgrades {
	BulletSpeed,
	BulletSize,
	ShootingSpeed,
	BulletDamager,
	BulletPierce
};


inline SpriteTexture toIcon(Upgrades upgrade) {
	switch (upgrade) {
		case Upgrades::BulletSpeed:   return ResourceManager::GetTexture("Bullet_speed");
		case Upgrades::BulletSize:    return ResourceManager::GetTexture("Bullet_dimension");
		case Upgrades::ShootingSpeed: return ResourceManager::GetTexture("Shooting_speed");
		case Upgrades::BulletDamager: return ResourceManager::GetTexture("Bullet_damage");
		case Upgrades::BulletPierce:  return ResourceManager::GetTexture("Bullet_Piercing");
	}
	return ResourceManager::GetTexture("Sconosciuto");
}

inline Upgrades randomUpgrade() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 4); 

	return static_cast<Upgrades>(dist(gen));
}

class Card : public Button {

public:
	Card(SpriteTexture icon, glm::vec2 position, glm::vec2 size, SpriteTexture texture,
		Upgrades upgrade,
		std::function<void(Upgrades)> onUpgradeChosen)  
		: Button(icon, size * 0.6f, position, size, texture,
			[upgrade, onUpgradeChosen]() {           
				onUpgradeChosen(upgrade);             
			}),
		upgrade(upgrade)
	{
		textScale = 0.4f;
	}


	Upgrades upgrade;
};


class UpgradeUI {
	Card* card1 = nullptr;
	Card* card2 = nullptr;
	Card* card3 = nullptr;
	
public:
	bool applied = false;
	UpgradeUI() {

	}
	~UpgradeUI() {
		destroyScene();
	}
	void update(GLFWwindow* window,float dt);
	void render(TextRenderer* textRenderer, SpriteRenderer* spriteRenderer);
	void createScene();
	void destroyScene();

	
};