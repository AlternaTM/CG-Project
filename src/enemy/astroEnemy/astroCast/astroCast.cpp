#include "astroCast.h"

//cats

void AstroCast::update(float dt) {
	ttl -= dt;
	if (ttl <= 0) {
		AstroCastManager::get_instance()->remove_cast(this);
	}
}
void AstroCast::render(FigRenderer& figRenderer,const glm::mat4& view) {

	figRenderer.drawRect(
		pos,
		size,
		angle,
		view,
		base_color // rosso
	);
}

// castManager
AstroCastManager* AstroCastManager::_INSTANCE = nullptr;

AstroCastManager* AstroCastManager::get_instance() {
	if (AstroCastManager::_INSTANCE == nullptr) {
		_INSTANCE = new AstroCastManager();
	}

	return _INSTANCE;
}

void AstroCastManager::update(float dt) {
	for (auto c : casts) {
		c->update(dt);
	}

	
}

AstroCast* AstroCastManager::spawn(glm::vec2 start, glm::vec2 target, glm::vec2 size, float ttl, const glm::vec4& color) {
	
	AstroCast* c = new AstroCast(start, size, ttl, color);

	casts.push_back(c);
	return c;
}

void AstroCastManager::render(FigRenderer& figRenderer, Camera& camera) {

	for (AstroCast* c : casts) {
		c->render(figRenderer,camera.getViewMatrix());
	}
}

void AstroCastManager::remove_cast(AstroCast* cast_ptr) {
	auto it = std::find_if(casts.begin(), casts.end(),
		[cast_ptr](AstroCast* e) { return e == cast_ptr; });

	if (it != casts.end()) {
		delete* it;
		casts.erase(it);
	}
}