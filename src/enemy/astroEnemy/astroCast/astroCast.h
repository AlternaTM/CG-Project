#pragma once
#include "entity.h"
#include <glm/glm.hpp>
#include <vector>
#include "figureRenderer/figureRenderer.h"
#include "camera/camera.h"

// CAST
class AstroCast {
private:
	glm::vec2 pos;
	glm::vec2 size;
	float ttl;
	float angle;
	glm::vec4 base_color;
public:
	AstroCast(glm::vec2 pos, glm::vec2 size, float ttl, const glm::vec4& base_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
		:pos(pos), size(size), ttl(ttl), base_color(base_color){
	};
	void update(float dt);
	void render(FigRenderer& figRenderer,const glm::mat4& view);
};

// MANAGER

class AstroCastManager {
private:
	AstroCastManager() {}
	static AstroCastManager* _INSTANCE;
	std::vector<AstroCast*> casts;

public:
	static AstroCastManager* get_instance();
	void update(float dt);
	AstroCast* spawn(glm::vec2 start, glm::vec2 target, glm::vec2 size, float ttl, const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	void render(FigRenderer& figRenderer, Camera& camera);
	void remove_cast(AstroCast* cast_ptr);
};