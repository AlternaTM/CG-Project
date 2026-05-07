#pragma once 
#include <glm/glm.hpp>


class Renderable {
public:
	virtual ~Renderable() = default;
	virtual glm::vec2 get_offset() = 0;
	virtual glm::vec2 get_frame_size() = 0;
};



class Animable : public Renderable{
	float timer = 0.0f;
public:
	
	uint8_t tot_framex; //totale dei frame presenti nel png su x
	uint8_t tot_rows = 1; //totale dei frame presenti nel png su x
	uint8_t max_frame; //massimo frame a cui deve arrivare questa animazione su x
	float frame_duration; //durata del singolo frame
	uint8_t actual_frame = 0; //frame attuale 
	uint8_t y_offset = 0; // da 0 a (ricorda che non parte da 1, ziopera, e anche che parte da sotto)
	bool flipped = false;

	virtual ~Animable() = default;
	virtual void on_animation_end() = 0;
	


	glm::vec2 get_offset() {
		float w = 1.0f / tot_framex;
		float x = flipped ? (actual_frame + 1) * w : actual_frame * w;
		
		return { x , y_offset * (1.0f / tot_rows) };
	}
	glm::vec2 get_frame_size() {
		float w = 1.0f / tot_framex;
		return { flipped ? -w : w, 1.0f / tot_rows };
	}

	void reset_anim() {
		timer = 0.0f;
		actual_frame = 0;
	}

	virtual void update_anim(float delta) {
		timer += delta;
		if (timer >= frame_duration) {
			timer = 0;
			//actual_frame = (actual_frame + 1) % max_frame;

			actual_frame++;
			if (actual_frame >= max_frame) {
				actual_frame = 0;       // o tienilo fermo se vuoi animazione one-shot
				on_animation_end();
			}
		}
	}




};

