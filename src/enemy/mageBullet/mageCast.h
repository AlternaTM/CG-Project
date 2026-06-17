#pragma once
#include "entity.h"
#include <glm/glm.hpp>
#include <vector>
#include "figureRenderer/figureRenderer.h"
#include "modelRenderer/modelRenderer.h"
#include "camera/camera.h"
#include "shader/shader.h"


enum CASTTYPE {
	MageCast,
	AstroCast,
	AstroShadowCast
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


class Asteroid : public Entity{
private: 
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
public: 
	Asteroid();
	glm::vec3 rotation_speed = { 0.0f, 0.0f, 0.0f };
	glm::vec3 get_rotation() {
		return rotation;
	}
	glm::vec3 get_scale() {
		return scale;
	}

	void set_rotation(glm::vec3 rotation) {
		Asteroid::rotation = rotation;
	}
	void set_scale(glm::vec3 scale) {
		Asteroid::scale = scale;
	}
};


// MANAGER

class CastManager {
private:

	CastManager() 
	{

	}
	static CastManager* _INSTANCE;
	std::vector<Cast*> casts;
	std::vector<Asteroid*> asteroids;

	FigRenderer* castRenderer = nullptr; 
	FigRenderer* astroRenderer = nullptr;
	FigRenderer* astroShadowRenderer = nullptr;
	ModelRenderer* asteroidModelRenderer = nullptr;

	~CastManager() {

	}

public:
	static CastManager* get_instance();
	static void init(FigRenderer* castRenderer, FigRenderer* astroRenderer, FigRenderer* astroShadowRenderer, ModelRenderer* asteroidModelRenderer);
	void update(float dt);

	void render( Camera& camera);
	void render_asteroi3d(Camera3D& camera,const glm::vec2 cam2dPos, const glm::mat4& projection3D);
	//void render_ateroids(Camera& camera);
	void add(Cast* cast);
	void remove(Cast* cast);

	void add_aster(Asteroid* asteroid);
	void remove_aster(Asteroid* asteroid);
};
