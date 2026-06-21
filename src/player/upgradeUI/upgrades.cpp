#include "upgrades.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "game/game.h"


void UpgradeUI::update(GLFWwindow* window, float dt){
	if (
		card1 == nullptr ||
		card2 == nullptr ||
		card3 == nullptr
		) {
		return;
	}

	Card* cards[3] = { card1, card2, card3 };


	for (Card* c : cards) {

		c->update(window);
	}

}

void UpgradeUI::render(TextRenderer* textRenderer, SpriteRenderer* spriteRenderer){
	if (
		card1 == nullptr ||
		card2 == nullptr ||
		card3 == nullptr
		) {
		return;
	}
	

	Card* cards[3] = { card1, card2, card3 };


	for (Card* c : cards) {

		c->render(*spriteRenderer, *textRenderer);
	}


	float scale = 0.7f;
	std::string text = "Choose";

	float textWidth = textRenderer->GetTextWidth(text, scale);
	float x = (1080 - textWidth);
	textRenderer->RenderText("Choose", 0.0f - textWidth/2.0f, 2.8f, scale, { 1.0f, 1.0f, 1.0f });

}


void UpgradeUI::createScene() {
	destroyScene();


	Upgrades up = randomUpgrade();
	card1 = new Card(toString(up), {-3.0f, 0.0f}, {2.0f, 3.0f},up , [this](Upgrades u) {
		//player.applyUpgrade(u);
		if (!applied && Game::get_instance()->get_chestManager().finished) {
			applied = true;
			Game::get_instance()->get_player()->apply_upgrade(u);
		}
	});
	up = randomUpgrade();
	card2 = new Card(toString(up), {0.0f, 0.0f}, {2.0f, 3.0f},up, [this](Upgrades u) {
		if (!applied && Game::get_instance()->get_chestManager().finished) {
			applied = true;
			Game::get_instance()->get_player()->apply_upgrade(u);
		}
		//player.applyUpgrade(u);
	});
	up = randomUpgrade();
	card3 = new Card(toString(up),{ 3.0f, 0.0f }, {2.0f, 3.0f},up, [this](Upgrades u) {
		//player.applyUpgrade(u);
		if (!applied && Game::get_instance()->get_chestManager().finished) {
			applied = true;
			Game::get_instance()->get_player()->apply_upgrade(u);
		}
		
	});
}

void UpgradeUI::destroyScene() {
	delete card1;
	card1 = nullptr;
	delete card2;
	card2 = nullptr;
	delete card3;
	card3 = nullptr;
}

