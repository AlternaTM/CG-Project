#include "game.h"
#include "../enemy/mageBullet/mageCast.h"
#include "../resourceManager/resourceManager.h"
// ================== InGameState ==========================
void InGameState::enter(Game& game) {

}
void InGameState::update(Game& game, float dt) {




	game.get_player()->update(dt,game.get_window());

	game.get_CastManager()->update(dt);
	game.get_enemyManager()->update(*game.get_player(), dt);

}
void InGameState::exit(Game& game) {

}


// ================== InLootingState ==========================
void LootingGameState::enter(Game& game) {

}
void LootingGameState::update(Game& game, float dt) {

	game.get_player()->update(dt, game.get_window());

	//game.get_CastManager()->update(dt);
	//game.get_enemyManager()->update(*game.get_player(), dt);

}
void LootingGameState::exit(Game& game) {

}

// ================== GAME MANAGER ==========================

Game* Game::_INSTANCE = nullptr;


Game::Game(GLFWwindow* window, Camera& camera, SpriteRenderer* renderer, FigRenderer* figRectRenderer, FigRenderer* figCastRenderer)
	: 
	player(camera), 
	castManager(CastManager::get_instance()),
	figCastRenderer(figCastRenderer),
	camera(camera),
	chestManager(2),
	renderer(renderer),
	enemyManager(EnemyManager::get_instance()),
	figRectRenderer(figRectRenderer),
	window(window)
{
	game_state = &inGameState;
}

void Game::init(GLFWwindow* window, Camera& camera, SpriteRenderer* renderer, FigRenderer* figRectRenderer, FigRenderer* figCastRenderer) {
	static Game game(window,camera,renderer,figRectRenderer,figCastRenderer);

	

	game.enemyManager->spawn_enemy(EnemyTipe::Mage, 3);
	game.enemyManager->spawn_enemy(EnemyTipe::Skeleton, 20);
	EnemyManager::_PLAYER = game.get_player();

	_INSTANCE = &game;
}


void Game::update(float dt) {
	game_state->update(*this, dt);
	chestManager.interact(window, player);
}

void Game::render() {
	enemyManager->render(*renderer, *figRectRenderer, camera);

	chestManager.render(*renderer, camera);

	(*renderer).Draw(
		ResourceManager::GetTexture("player"),
		*player.get_pos(),
		*player.get_size(),
		0.0f,
		camera.getViewMatrix(),
		player.get_offset(),
		player.get_frame_size()
	);

	castManager->render(*figCastRenderer, camera);

	(*renderer).Draw(
		ResourceManager::GetTexture("arrow"),
		player.aimPosition,
		{ 1.0f, 1.0f },
		player.aimRotation,
		camera.getViewMatrix()
	);
}

void Game::switch_state(GameStateType state) {
	if (state == game_state->get_type()) {
		return;
	}
	game_state->exit(*this);
	switch (state) {
	case GameStateType::InGame:
		game_state = &inGameState;
		break;
	case GameStateType::Looting:
		game_state = &lootingState;
		break;
	case GameStateType::Pause:
		break;
	}
	game_state->enter(*this);
}

Player* Game::get_player() {
	return &player;
}


GLFWwindow* Game::get_window() {
	return window;
}

CastManager* Game::get_CastManager() {
	return castManager;
}
EnemyManager* Game::get_enemyManager() {
	return enemyManager;
}


Game* Game::get_instance() {
	assert(_INSTANCE != nullptr);
	return _INSTANCE;
}