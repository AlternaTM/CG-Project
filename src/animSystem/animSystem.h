#pragma once 
#include <glm/glm.hpp>


class Renderable {
public:
	virtual ~Renderable() = default;
	virtual glm::vec2 get_offset() = 0;
	virtual glm::vec2 get_frame_size() = 0;
};



class Animable : public Renderable{
private:
	float timer = 0.0f;
public:
	uint8_t tot_frame; //totale dei frame presenti nel png
	uint8_t max_frame; //massimo frame a cui deve arrivare questa animazione
	float frame_duration; //durata del singolo frame
	uint8_t actual_frame = 0; //frame attuale 

	virtual ~Animable() = default;
	virtual void on_animation_end() = 0;


	glm::vec2 get_offset() {
		return { actual_frame * (1.0f / tot_frame) ,0.0f };
	}
	glm::vec2 get_frame_size() {
		return { 1.0f / tot_frame,1.0f };
	}

	virtual void update_anim(float delta) {
		timer += delta;
		if (timer >= frame_duration) {
			timer = 0;
			actual_frame = (actual_frame + 1) % tot_frame;

			if (actual_frame == max_frame) {
				on_animation_end(); 
			}
		}
	}

};

