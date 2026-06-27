#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include "spriteRenderer/spriteRenderer.h"
#include "camera/camera.h"
#include "animSystem/animSystem.h"
#include "entity.h"
#include <vector>
#include "enemy/enemy.h"



class Bullet : public Entity, public Renderable {
public: 
	
	uint8_t damage; 
	float speed;
	glm::vec2 direction;
	uint8_t hit = 0;
	uint8_t max_hit = 1;

	Enemy* lastHit = nullptr;

	Bullet(glm::vec2 direction, uint8_t damage, float speed, uint8_t max_hit = 1, float size_multi = 1.0f) :
		direction(direction), damage(damage), speed(speed), max_hit (max_hit)
	{
		size = glm::vec2(0.3f * size_multi);
	};

	virtual void update(float dt);
	virtual std::string get_name() {
		return "bullet";
	}

	void set_position(glm::vec3 position) {
		pos = position;
	}

	virtual glm::vec2 get_offset() {
		return glm::vec2(0.0f);
	}
	virtual glm::vec2 get_frame_size() {
		return glm::vec2(1.0f);
	}
};



//============ MANAGER

class BulletManager {
	std::vector<Bullet*> bullets;
	BulletManager() {}
	static BulletManager* _INSTANCE;
public:


	~BulletManager() {
		remove_all();
	}

	void update(float dt);
	void render(SpriteRenderer& enemyRenderer, Camera& camera);

	static void add_bullet(Bullet* bullet);
	void remove_bullet(Bullet* bullet);
	void remove_all();

	static BulletManager* get_instance();
	void reset();
};