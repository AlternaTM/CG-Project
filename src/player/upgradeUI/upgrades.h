#pragma once

#include <glm/glm.hpp>
#include <string>
#include "spriteRenderer/spriteRenderer.h"
#include "camera/camera.h"
#include "figureRenderer/figureRenderer.h"
#include <glm/glm.hpp>
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


inline std::string toString(Upgrades upgrade) {
	switch (upgrade) {
	case Upgrades::BulletSpeed:   return "Bullet speed";
	case Upgrades::BulletSize:    return "Bullet dimension";
	case Upgrades::ShootingSpeed: return "Shooting speed";
	case Upgrades::BulletDamager: return "Bullet damage";
	case Upgrades::BulletPierce: return "Bullet Piercing";
	}
	return "Sconosciuto";
}

inline Upgrades randomUpgrade() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 3); 

	return static_cast<Upgrades>(dist(gen));
}

class Card : public Button {

public:
	Card(const std::string& label, glm::vec2 position, glm::vec2 size,
		Upgrades upgrade,
		std::function<void(Upgrades)> onUpgradeChosen)  
		: Button(label, position, size,
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