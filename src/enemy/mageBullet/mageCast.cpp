#include "mageCast.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
//cats

void Cast::update(float dt) {

}
void Cast::render(FigRenderer& figRenderer,const glm::mat4& view) {

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

void CastManager::render(FigRenderer& castRenderer, FigRenderer& astroRenderer, Camera& camera) {

	for (Cast* c : casts) {
		switch (c->get_type()) {
			case CASTTYPE::MageCast:
				c->render(castRenderer, camera.getViewMatrix());
				break;
			case CASTTYPE::AstroCast:
				c->render(astroRenderer, camera.getViewMatrix());
				break;
		}
		
		
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
