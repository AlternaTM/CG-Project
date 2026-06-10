#pragma once
#include "entity.h"
#include <glm/glm.hpp>
#include <vector>
#include "figureRenderer/figureRenderer.h"
#include "camera/camera.h"

enum CASTTYPE {
	MageCast,
	AstroCast
};

// CAST
class Cast {
private:
	Mesh2D mesh;
	glm::mat4 model;
	glm::vec4 base_color;
	CASTTYPE type;
public:

	Cast(Mesh2D mesh,glm::mat4 model,CASTTYPE type, const glm::vec4& base_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
		:mesh(mesh), model(model), base_color(base_color), type(type){
	};
	~Cast();
	void update(float dt);
	void render(FigRenderer& figRenderer,const glm::mat4& view);
	void update_model(glm::mat4 model);
	CASTTYPE get_type() {
		return type;
	}
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

	void render(FigRenderer& castRenderer, FigRenderer& astroRenderer, Camera& camera);
	void add(Cast* cast);
	void remove(Cast* cast);
};
