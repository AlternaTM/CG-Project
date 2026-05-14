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
	glm::vec4 base_color;
public:
	Cast(glm::vec2 start_pos, glm::vec2 size, float ttl, float angle, const glm::vec4& base_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
		:start_pos(start_pos), size(size), ttl(ttl), angle(angle), base_color(base_color){
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
	Cast* spawn(glm::vec2 start, glm::vec2 target, glm::vec2 size, float ttl, const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	void render(FigRenderer& figRenderer, Camera& camera);
	void remove_cast(Cast* cast_ptr);
};