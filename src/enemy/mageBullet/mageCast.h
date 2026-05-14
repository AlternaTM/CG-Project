#pragma once
#include "entity.h"
#include <glm/glm.hpp>
#include <vector>
#include "figureRenderer/figureRenderer.h"
#include "camera/camera.h"

// CAST
class Cast {
private:
	glm::vec2 start_pos;
	glm::vec2 size;
	float ttl;
	float angle;
public:
	Cast(glm::vec2 start_pos, glm::vec2 size, float ttl, float angle)
		:start_pos(start_pos), size(size), ttl(ttl), angle(angle) {
	};
	void update(float dt);
	void render(FigRenderer& figRenderer,const glm::mat4& view);
};

// MANAGER

class CastManager {
private:
	CastManager() {}
	static CastManager* _INSTANCE;
	std::vector<Cast*> casts;

public:
	static CastManager* get_instance();
	void update(float dt);
	void spawn(glm::vec2 start, glm::vec2 target, glm::vec2 size, float ttl);
	void render(FigRenderer& figRenderer, Camera& camera);
	void remove_cast(Cast* cast_ptr);
};