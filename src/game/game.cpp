#include "game.h"
#include "../enemy/mageBullet/mageCast.h"
#include "../resourceManager/resourceManager.h"
#include "../modelRenderer/modelRenderer.h"

// ================== TitleState ==========================
void TitleGameState::enter(Game& game) {
	game.switch_state(GameStateType::Title);

}

void TitleGameState::update(Game& game, float dt) {
	for (auto& b : game.get_buttons(GameStateType::Title)) {
		b.update(game.get_window());
	}

}

void TitleGameState::exit(Game& game) {

}

void TitleGameState::renderUI(Game& game) {
	game.get_SpriteRenderer()->Draw(
		ResourceManager::GetTexture("titleScreen"),
		{ 0.0f, 0.0f },              // centro
		{ 16.0f, 9.0f },
		0.0f,
		glm::mat4(1.0f)
	);

	for (auto& b : game.get_buttons(GameStateType::Title)) {
		b.render(*game.get_SpriteRenderer(), *game.get_TextRenderer());
	}

}

// ================== InGameState ==========================
void InGameState::enter(Game& game) {

}

void InGameState::update(Game& game, float dt) {
	if (PlayerInput::isKeyJustPressed(game.get_window(), GLFW_KEY_ESCAPE)) {
		game.switch_state(GameStateType::Pause);
	}
	game.get_player()->update(dt,game.get_window());
	game.get_timer()->update(dt);

	game.get_CastManager()->update(dt);
	game.get_enemyManager()->update(*game.get_player(), dt);
	game.get_bulletManager()->update(dt);

}

void InGameState::exit(Game& game) {

}

void InGameState::renderUI(Game& game) {
	int min = game.get_timer()->getMinutes();
	int sec = game.get_timer()->getSeconds();

	char buffer[6];
	sprintf_s(buffer, "%02d:%02d", min, sec);

	game.get_TextRenderer()->RenderText(buffer, -7.5f, 3.7f, 0.7f, { 1.0f, 1.0f, 1.0f });
}


// ================== InLootingState ==========================
void LootingGameState::enter(Game& game) {
	game.upgradeUI.createScene();
	game.get_chestManager().angle = 0.0f;
	game.get_chestManager().finished = false;
}

void LootingGameState::update(Game& game, float dt) {
	game.upgradeUI.update(game.get_window(),dt);

	if (game.upgradeUI.applied) {
		game.upgradeUI.applied = false;
		game.switch_state(GameStateType::InGame);
	}
}

void LootingGameState::exit(Game& game) {
	game.upgradeUI.destroyScene();
	game.get_chestManager().finished = false;
}

void LootingGameState::render2d(Game& game) {
	game.get_SpriteRenderer()->DrawColor(
		{ 0.0f, 0.0f },
		{ 32.0f, 18.0f },
		{ 0.0f, 0.0f, 0.0f, 0.7f }
	);

}

void LootingGameState::render3d(Game& game,float dt) {

	game.get_chestManager().render_chest(
		*game.get_camera3D(),
		game.get_projection3D(),
		*game.get_chest_part()[0],
		*game.get_chest_part()[1],
		dt
	);

}

void LootingGameState::renderUI(Game& game) {
	int min = game.get_timer()->getMinutes();
	int sec = game.get_timer()->getSeconds();

	char buffer[6];
	sprintf_s(buffer, "%02d:%02d", min, sec);

	game.get_TextRenderer()->RenderText(buffer, -7.5f, 3.7f, 0.7f, { 1.0f, 1.0f, 1.0f });

	if (game.get_chestManager().finished) {
		game.upgradeUI.render(game.get_TextRenderer(), game.get_SpriteRenderer());
	}
}


// ================== PauseState ==========================
void PauseGameState::enter(Game& game) {

}

void PauseGameState::update(Game& game, float dt) {

	if (PlayerInput::isKeyJustPressed(game.get_window(), GLFW_KEY_ESCAPE)) {
		game.switch_state(GameStateType::InGame);
	}

	for (auto& b : game.get_buttons(GameStateType::Pause)) {
		b.update(game.get_window());
	}
}

void PauseGameState::exit(Game& game) {

}

void PauseGameState::renderUI(Game& game) {
	int min = game.get_timer()->getMinutes();
	int sec = game.get_timer()->getSeconds();

	char buffer[6];
	sprintf_s(buffer, "%02d:%02d", min, sec);

	game.get_TextRenderer()->RenderText(buffer, -7.5f, 3.7f, 0.7f, { 1.0f, 1.0f, 1.0f });

	game.get_SpriteRenderer()->DrawColor(
		{ 0.0f, 0.0f },
		{ 32.0f, 18.0f },
		{ 0.0f, 0.0f, 0.0f, 0.7f }
	);

	for (auto& b : game.get_buttons(GameStateType::Pause)) {
		b.render(*game.get_SpriteRenderer(), *game.get_TextRenderer());
	}
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
	chestManager(),
	renderer(renderer),
	enemyManager(EnemyManager::get_instance()),
	window(window),
	camera3D(camera3D),
	projection3D(projection3D),
	chest_part(chest_part)
{
	game_state = &titleState;

	
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

	//game.enemyManager->spawn_enemy(EnemyTipe::Mage, 1);+
	game.bulletManager = BulletManager::get_instance();
	game.init_renderers(projection);
	game.init_buttons();

	game.castManager->init(&game.figCastRenderer, &game.figAstroCastRenderer, &game.figAstroShadowCastRenderer, &game.asteroidModelRenderer);


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

	textRenderer.LoadFont("./assets/fonts/PressStart2P-Regular.ttf", 48);

	return 0;
}

void Game::init_buttons() {
	stateButtons[GameStateType::Title] = {
		Button(
			"INIZIA",
			glm::vec2(0.0f, 1.0f),
			glm::vec2(5.0f, 1.3f),
			[this]() {spawn_game();  switch_state(GameStateType::InGame); }
		),
		Button(
			"ESCI",
			glm::vec2(0.0f, -1.0f),
			glm::vec2(5.0f, 1.3f),
			[this]() {  }
		)
	};

	stateButtons[GameStateType::Pause] = {
		Button(
			"RIPRENDI",
			glm::vec2(0.0f, 1.0f),
			glm::vec2(5.0f, 1.3f),
			[this]() {  switch_state(GameStateType::InGame); }
		),
		Button(
			"ABBANDONA",
			glm::vec2(0.0f, -1.0f),
			glm::vec2(5.0f, 1.3f),
			[this]() {reset(); switch_state(GameStateType::Title); }
		)
	};

	stateButtons[GameStateType::GameOver] = {
		Button(
			"RIPROVA",
			glm::vec2(0.0f, 1.0f),
			glm::vec2(5.0f, 1.3f),
			[this]() { switch_state(GameStateType::InGame); }
		),
		Button(
			"ABBANDONA",
			glm::vec2(0.0f, -1.0f),
			glm::vec2(5.0f, 1.3f),
			[this]() { switch_state(GameStateType::Title); }
		)
	};
}

void Game::update(float dt) {
	game_state->update(*this, dt);
	if (chestManager.interact(window, player)) {
		this->switch_state(GameStateType::Looting);
	}
}

void Game::render2d() {

	chestManager.render(*renderer, camera);
	enemyManager->render(*renderer, figRectRenderer, camera);

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

	bulletManager->render(*renderer, camera);

	(*renderer).Draw(
		ResourceManager::GetTexture("arrow"),
		player.aimPosition,
		{ 1.0f, 1.0f },
		player.aimRotation,
		camera.getViewMatrix()
	);

	game_state->render2d(*this);
}

void Game::render3d(float dt) {
	castManager->render_asteroi3d(*camera3D, camera.getCameraPosition(), projection3D);
	game_state->render3d(*this,dt);
	
}

void Game::renderUI() {
	game_state->renderUI(*this);
		
}

void Game::switch_state(GameStateType state) {
	if (state == game_state->get_type()) {
		return;
	}
	game_state->exit(*this);
	switch (state) {
	case GameStateType::Title:
		game_state = &titleState;
		break;
	case GameStateType::InGame:
		game_state = &inGameState;
		break;
	case GameStateType::Looting:
		game_state = &lootingState;
		break;
	case GameStateType::Pause:
		game_state = &pauseState;
		break;
	}
	game_state->enter(*this);
}

Player* Game::get_player() {
	return &player;
}

Timer* Game::get_timer() {
	return &timer;
}

GLFWwindow* Game::get_window() {
	return window;
}

SpriteRenderer* Game::get_SpriteRenderer() {
	return renderer;
}

TextRenderer* Game::get_TextRenderer() {
	return &textRenderer;
}

CastManager* Game::get_CastManager() {
	return castManager;
}

EnemyManager* Game::get_enemyManager() {
	return enemyManager;
}

BulletManager* Game::get_bulletManager() {
	return bulletManager;
}

ChestManager& Game::get_chestManager() {
	return chestManager;
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

std::vector<Button>& Game::get_buttons(GameStateType type) {
	return stateButtons[type];
}


void Game::reset() {
	player.reset();
	enemyManager->reset();
	chestManager.reset();
	bulletManager->reset();
}

void Game::spawn_game() {
	enemyManager->spawn_enemy(EnemyTipe::Skeleton, 10);
	enemyManager->spawn_enemy(EnemyTipe::Astro, 1);
	chestManager.spawn_chest();
	chestManager.spawn_chest();
}