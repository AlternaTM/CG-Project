#include "mageCast.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "modelRenderer/modelRenderer.h"
#include <glm/gtc/quaternion.hpp>
#include <algorithm>
//cats

void Cast::update(float dt) {

}
void Cast::render(FigRenderer& figRenderer,const glm::mat4& view, const float uTime) {

	figRenderer.draw(mesh, model, view, base_color, uTime);
}

void Cast::update_model(glm::mat4 model) {
	Cast::model = model;
}


Cast::~Cast() {
	mesh.destroy();
}


//Asteroid

Asteroid::Asteroid() 
{
	
}




// castManager

CastManager* CastManager::_INSTANCE = nullptr;

CastManager* CastManager::get_instance() {
	if (CastManager::_INSTANCE == nullptr) {
		_INSTANCE = new CastManager();

		//ASTRO 

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

void CastManager::init(FigRenderer* castRenderer, FigRenderer* astroRenderer, FigRenderer* astroShadowRenderer, ModelRenderer* asteroidModelRenderer) {
	CastManager::_INSTANCE->castRenderer = castRenderer;
	CastManager::_INSTANCE->astroRenderer = astroRenderer;
	CastManager::_INSTANCE->astroShadowRenderer = astroShadowRenderer;
	CastManager::_INSTANCE->asteroidModelRenderer = asteroidModelRenderer;
}


void CastManager::render(Camera& camera) {
	
	for (Cast* c : casts) {
		
		switch (c->get_type()) {
			case CASTTYPE::MageCast:
				c->render(*castRenderer, camera.getViewMatrix(), (float)glfwGetTime());
				break;
			case CASTTYPE::AstroCast:
				c->render(*astroRenderer, camera.getViewMatrix(),  (float)glfwGetTime());
				break;
			case CASTTYPE::AstroShadowCast:
				c->render(*astroShadowRenderer, camera.getViewMatrix(), ( (float)glfwGetTime() - c->startTime) / 4.0f );
				break;
		}
		
		
	}
}

void CastManager::render_asteroi3d(Camera3D& camera,const glm::vec2 cam2dPos, const glm::mat4& projection3D) {
	for (Asteroid* as : asteroids) {

		glm::mat4 parentMatrix = glm::mat4(1.0f);
		parentMatrix = glm::translate(parentMatrix, *as->get_pos()); 
		parentMatrix = glm::scale(parentMatrix,	as->get_scale()); 

		glm::quat qX = glm::angleAxis(glm::radians(as->get_rotation().x), glm::vec3(1, 0, 0));
		glm::quat qY = glm::angleAxis(glm::radians(as->get_rotation().y), glm::vec3(0, 1, 0));
		glm::quat qZ = glm::angleAxis(glm::radians(as->get_rotation().z), glm::vec3(0, 0, 1));

		parentMatrix = parentMatrix * glm::mat4_cast(qZ * qY * qX);


		Shader* shader = asteroidModelRenderer->shader;
		shader->use();
		shader->setVec3("viewPos", camera.position);

		glm::mat4 view3D = camera.getViewMatrix();
		view3D = glm::translate(view3D, glm::vec3(-cam2dPos.x, -cam2dPos.y, 0.0f));

		asteroidModelRenderer->render(view3D, projection3D,&parentMatrix);
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


void CastManager::add_aster(Asteroid* asteroid) {
	asteroids.push_back(asteroid);
}
void CastManager::remove_aster(Asteroid* asteroid_ptr) {
	auto it = std::find_if(asteroids.begin(), asteroids.end(),
		[asteroid_ptr](Asteroid* e) { return e == asteroid_ptr; });

	if (it != asteroids.end()) {
		//delete* it;
		asteroids.erase(it);
	}
}
