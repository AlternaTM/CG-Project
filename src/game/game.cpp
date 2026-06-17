#include "game.h"
#include "../enemy/mageBullet/mageCast.h"
#include "../resourceManager/resourceManager.h"
#include "../modelRenderer/modelRenderer.h"
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
	if (PlayerInput::isKeyJustPressed(game.get_window(), GLFW_KEY_E)) {
		game.switch_state(GameStateType::InGame);
	}
}
void LootingGameState::exit(Game& game) {

}

void LootingGameState::render3d(Game& game,float dt) {
	
	ModelRenderer::render_chest(
		*game.get_camera3D(),
		game.get_projection3D(),
		*game.get_chest_part()[0],
		*game.get_chest_part()[1],
		dt
	);

	
}

// ================== GAME MANAGER ==========================

Game* Game::_INSTANCE = nullptr;


Game::Game(
	GLFWwindow* window, 
	Camera& camera, 
	SpriteRenderer* renderer, 
	Camera3D* camera3D,
	const glm::mat4 projection3D,
	const std::array<ModelRenderer*, 2>& chest_part
)
	: 
	player(camera), 
	castManager(CastManager::get_instance()),
	camera(camera),
	chestManager(2),
	renderer(renderer),
	enemyManager(EnemyManager::get_instance()),
	window(window),
	camera3D(camera3D),
	projection3D(projection3D),
	chest_part(chest_part)
{
	game_state = &inGameState;
}

void Game::init(
	GLFWwindow* window,
	Camera& camera,
	const glm::mat4& projection,
	SpriteRenderer* renderer,
	Camera3D* camera3D,
	const glm::mat4 projection3D,
	const std::array<ModelRenderer*, 2>& chest_part
) {
	static Game game(
		window,
		camera,
		renderer,
		camera3D,
		projection3D,
		chest_part
	);

	

	//game.enemyManager->spawn_enemy(EnemyTipe::Mage, 1);
	game.init_renderers(projection);

	game.castManager->init(&game.figCastRenderer, &game.figAstroCastRenderer, &game.figAstroShadowCastRenderer, &game.asteroidModelRenderer);

	game.enemyManager->spawn_enemy(EnemyTipe::Skeleton, 10);
	game.enemyManager->spawn_enemy(EnemyTipe::Astro, 1);
	EnemyManager::_PLAYER = game.get_player();

	

	_INSTANCE = &game;
}


int Game::init_renderers(const glm::mat4& projection) {

	if (!figRectRenderer.init(projection))
		return 1;

	if (!figCastRenderer.init(projection)) {
		return 1;
	}

	if (!figAstroCastRenderer.init(projection)) {
		return 1;
	}

	if (!figAstroShadowCastRenderer.init(projection)) {
		return 1;
	}

	return 0;
}


void Game::update(float dt) {
	game_state->update(*this, dt);
	if (chestManager.interact(window, player)) {
		this->switch_state(GameStateType::Looting);
	}
}

void Game::render2d() {
	enemyManager->render(*renderer, figRectRenderer, camera);

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

	castManager->render(camera);

	(*renderer).Draw(
		ResourceManager::GetTexture("arrow"),
		player.aimPosition,
		{ 1.0f, 1.0f },
		player.aimRotation,
		camera.getViewMatrix()
	);

	
}

void Game::render3d(float dt) {
	game_state->render3d(*this,dt);
	castManager->render_asteroi3d(*camera3D,camera.getCameraPosition(),  projection3D);
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


Camera3D* Game::get_camera3D() {
	return camera3D;
}
const glm::mat4 Game::get_projection3D() {
	return projection3D;
}


std::array<ModelRenderer*, 2>& Game::get_chest_part() {
	return chest_part;
}