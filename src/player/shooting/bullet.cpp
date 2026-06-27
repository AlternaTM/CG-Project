#include "bullet.h"
#include "resourceManager/resourceManager.h"
#include "collision/collision.h"

#include "game/game.h"
#include <cmath>

static constexpr float RAD2DEG = 57.2957795f;

BulletManager* BulletManager::_INSTANCE = nullptr;


void Bullet::update(float dt) {
	Entity::pos += glm::vec3(direction * speed * dt, 0.0f);
}



//============ MANAGER

void BulletManager::add_bullet(Bullet* bullet) {
	if (bullet != nullptr) {
		BulletManager::_INSTANCE->bullets.push_back(bullet);
	}
}

void BulletManager::remove_bullet(Bullet* bullet) {
	auto it = std::find(bullets.begin(), bullets.end(), bullet);

	if (it != bullets.end()) {
		delete* it;
		bullets.erase(it);
	}
}

void BulletManager::render(SpriteRenderer& enemyRenderer, Camera& camera) {
	for (Bullet* bullet : bullets) {

		float rot = atan2f(
			bullet->direction.y,
			bullet->direction.x
		) ;

		enemyRenderer.Draw(
			ResourceManager::GetTexture(bullet->get_name()),
			*bullet->get_pos(),
			*bullet->get_size(),
			rot,
			camera.getViewMatrix(),
			bullet->get_offset(),
			bullet->get_frame_size()
		);
	}
}


void BulletManager::update(float dt) {
	for (Bullet* bullet : bullets) {
		bullet->update(dt);

		for (Enemy* e : EnemyManager::get_instance()->get_enemys()) {
			Entity fly; 
			fly.get_pos()->x = e->get_pos()->x;
			fly.get_pos()->y = e->get_pos()->y;
			fly.get_size()->x = e->get_size()->x * 0.3f;
			fly.get_size()->y = e->get_size()->y * 0.5f;
			if (CollisionChecker::check_collision(fly, *bullet)) {
				if (bullet->lastHit != e) {
					std::cout << "COLPITO:   " << bullet->lastHit << " ---- " << e << std::endl;
					bullet->lastHit = e;
					bullet->hit += 1;
					e->make_damage(bullet->damage);
				}
			}
		}
		
	}

	

	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(), [](Bullet* b) {
				bool toRemove = b->hit >= b->max_hit; 
				toRemove = toRemove || (b->get_pos()->x > 18.0f) || (b->get_pos()->x < -18.0f) || (b->get_pos()->y > 15.0f) || (b->get_pos()->y < -15.0f);
				if (toRemove) {
					delete b;
				};
				return toRemove;
			}),
			bullets.end()
	);
}


void BulletManager::remove_all() {
	for (Bullet* b : bullets) {
		delete b;
	}

	bullets.clear();
}


BulletManager* BulletManager::get_instance() {
	if (_INSTANCE == nullptr) {
		_INSTANCE = new BulletManager();
	}

	return _INSTANCE;
}

void BulletManager::reset() {
	remove_all();
}