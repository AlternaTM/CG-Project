#pragma once
#include <glm/glm.hpp>

class Entity {
protected: 
	glm::vec3 pos = { 0.0f,0.0f,0.0f };
	glm::vec2 size = { 1.0f,1.0f };
public: 
	virtual ~Entity() = default;
	virtual glm::vec3* get_pos() {
		return &pos;
	}
	virtual glm::vec2* get_size() {
		return &size;
	}


};