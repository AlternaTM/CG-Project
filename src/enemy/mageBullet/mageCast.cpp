#include "mageCast.h"

//cats

void Cast::update(float dt) {
	ttl -= dt;
	if (ttl <= 0) {
		CastManager::get_instance()->remove_cast(this);
	}
}
void Cast::render(FigRenderer& figRenderer,const glm::mat4& view) {

	figRenderer.drawRect(
		start_pos,
		size,
		angle,
		view,
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) // rosso
	);
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

void CastManager::spawn(glm::vec2 start, glm::vec2 target, glm::vec2 size, float ttl) {
	glm::vec2 dir = glm::normalize(target - start);

	float angle = atan2(dir.y, dir.x);
	Cast* c = new Cast(start, size, ttl, angle);

	casts.push_back(c);
}

void CastManager::render(FigRenderer& figRenderer, Camera& camera) {

	for (Cast* c : casts) {
		c->render(figRenderer,camera.getViewMatrix());
	}
}

void CastManager::remove_cast(Cast* cast_ptr) {
	auto it = std::find_if(casts.begin(), casts.end(),
		[cast_ptr](Cast* e) { return e == cast_ptr; });

	if (it != casts.end()) {
		delete* it;
		casts.erase(it);
	}
}