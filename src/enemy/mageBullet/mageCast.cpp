#include "mageCast.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
//cats

void Cast::update(float dt) {

}
void Cast::render(FigRenderer& figRenderer,const glm::mat4& view) {

	/*
	if (!meshReady) {
		std::vector<Vertex2D> vertices = {
			{{-0.5f, -0.5f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f}, {0.0f, 1.0f}},
		};
		std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
		mesh.create(vertices, indices);
		meshReady = true;
	}

	float elapsed = initialttl - ttl;
	float maxLength = size.x; //8.0f;
	float speed = 12.0f;
	float beamWidth = size.y;//0.28f;


	float currentLength = glm::min(elapsed * speed, maxLength);



	glm::vec2 forward = glm::vec2(cos(angle), sin(angle));
	glm::vec2 beamCenter = start_pos + forward * (currentLength * 0.5f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(beamCenter, 0.0f));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(currentLength, beamWidth, 1.0f));

	figRenderer.draw(mesh, model, view, base_color);
	*/


	figRenderer.draw(mesh, model, view, base_color);
}

void Cast::update_model(glm::mat4 model) {
	Cast::model = model;
}


Cast::~Cast() {
	mesh.destroy();
}

// castManager

CastManager* CastManager::_INSTANCE = nullptr;

CastManager* CastManager::get_instance() {
	if (CastManager::_INSTANCE == nullptr) {
		_INSTANCE = new CastManager();
	}

	return _INSTANCE;
}

void CastManager::update(float dt) {
	for (auto c : casts) {
		c->update(dt);
	}

	
}

void CastManager::add(Cast* cast) {
	casts.push_back(cast);
}

void CastManager::render(FigRenderer& figRenderer, Camera& camera) {

	for (Cast* c : casts) {
		c->render(figRenderer,camera.getViewMatrix());
	}
}

void CastManager::remove(Cast* cast_ptr) {
	auto it = std::find_if(casts.begin(), casts.end(),
		[cast_ptr](Cast* e) { return e == cast_ptr; });

	if (it != casts.end()) {
		//delete* it;
		casts.erase(it);
	}
}
