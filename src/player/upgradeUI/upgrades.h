#pragma once

#include <glm/glm.hpp>
#include "spriteRenderer/spriteRenderer.h"
#include "resourceManager/resourceManager.h"
#include "textRenderer/textRenderer.h"
#include "button/button.h"
#include <random>


class FigRenderer;
enum Upgrades {
	BulletSpeed,
	BulletSize,
	ShootingSpeed,
	BulletDamager,
	BulletPierce,
	HealtPlus
};


inline SpriteTexture toIcon(Upgrades upgrade) {
	switch (upgrade) {
		case Upgrades::BulletSpeed:   return ResourceManager::GetTexture("Bullet_speed");
		case Upgrades::BulletSize:    return ResourceManager::GetTexture("Bullet_dimension");
		case Upgrades::ShootingSpeed: return ResourceManager::GetTexture("Shooting_speed");
		case Upgrades::BulletDamager: return ResourceManager::GetTexture("Bullet_damage");
		case Upgrades::BulletPierce:  return ResourceManager::GetTexture("Bullet_Piercing");
		case Upgrades::HealtPlus:     return ResourceManager::GetTexture("Health_Plus");
	}
	return ResourceManager::GetTexture("Sconosciuto");
}

inline Upgrades randomUpgrade() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 5); 

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