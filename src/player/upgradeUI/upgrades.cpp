#include "upgrades.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

void UpgradeUI::update(float dt){
	if (
		card1 == nullptr ||
		card2 == nullptr ||
		card3 == nullptr
		) {
		return;
	}



}

void UpgradeUI::render(FigRenderer* renderer, Camera& camera){
	if (
		card1 == nullptr ||
		card2 == nullptr ||
		card3 == nullptr
		) {
		return;
	}
	
	static std::vector<Vertex2D> vertices = {
	{{-0.5f, -0.5f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f}, {1.0f, 0.0f}},
	{{ 0.5f,  0.5f}, {1.0f, 1.0f}},
	{{-0.5f,  0.5f}, {0.0f, 1.0f}},
	};

	static std::vector<uint32_t> indices = {
		0, 1, 2,
		2, 3, 0
	};

	Card* cards[3] = { card1, card2, card3 };


	for (Card* c : cards) {
		Mesh2D mesh;
		mesh.create(vertices, indices);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(c->pos, 0.0f));
		transform = glm::scale(transform, glm::vec3(c->size, 1.0f));

		renderer->draw(mesh, transform, glm::mat4(1.0f), glm::vec4(0.9f, 0.85f, 0.7f, 1.0f));
	}


}


void UpgradeUI::createScene() {
	destroyScene();
	card1 = new Card({ -5.0f, 0.0f }, { 2.0f, 3.0f });
	card2 = new Card({ 0.0f, 0.0f }, { 2.0f, 3.0f });
	card3 = new Card({ 5.0f, 0.0f }, { 2.0f, 3.0f });

}

void UpgradeUI::destroyScene() {
	delete card1;
	card1 = nullptr;
	delete card2;
	card2 = nullptr;
	delete card3;
	card3 = nullptr;
}