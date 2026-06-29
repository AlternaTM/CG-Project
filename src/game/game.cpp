#include "game.h"
#include "../enemy/mageBullet/mageCast.h"
#include "../resourceManager/resourceManager.h"
#include "../modelRenderer/modelRenderer.h"
#include "globals.h"
#include "../highscoreManager/highscoreManager.h"

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
	/*
	game.get_SpriteRenderer()->Draw(
		ResourceManager::GetTexture("titleScreen"),
		{ 0.0f, 0.0f },              // centro
		{ 16.0f, 9.0f },
		0.0f,
		glm::mat4(1.0f)
	);
	*/
	for (auto& b : game.get_buttons(GameStateType::Title)) {
		b.render(*game.get_SpriteRenderer(), *game.get_TextRenderer());
	}

	float scale = 0.9f;
	std::string text = "Battlefield in the Bedroom";

	float textWidth = game.get_TextRenderer()->GetTextWidth(text, scale);
	float x = (1080 - textWidth);
	game.get_TextRenderer()->RenderText("Battlefield in the Bedroom", -6.7f, 3.2f, scale, { 1.0f, 1.0f, 1.0f });

}

void TitleGameState::render3d(Game& game, float dt) {
	glm::mat4 bearMatrix = glm::mat4(1.0f);
	bearMatrix = glm::translate(bearMatrix, glm::vec3(0.20f, -0.208f, 0.68f)); 
	bearMatrix = glm::rotate(bearMatrix, glm::radians(37.47f), glm::vec3(0.0f, 1.0f, 0.0f));
	bearMatrix = glm::scale(bearMatrix, glm::vec3(0.68f));
	//glm::mat4 bearMatrix = positionerObjectHElper(game.get_window(), dt);

	//parentMatrix = glm::scale(parentMatrix, glm::vec3(0.2f));
	game.get_bear_model()->shader->use();

	game.get_bear_model()->shader->setVec3("lightPos1", glm::vec3(-2.0f, 0, 0));
	game.get_bear_model()->shader->setVec3("lightColor1", glm::vec3(1, 1, 1));

	//glm::mat4 lioghMatrix = positionerObjectHElper(game.get_window(), dt);
	
	game.get_bear_model()->shader->setVec3("lightPos2", glm::vec3(2.26f, 1.64f, 2.42f)); //  glm::vec3(2.2f, 2.0f, 1.6f)
	game.get_bear_model()->shader->setVec3("lightColor2", glm::vec3(0.6f, 0.6f, 0.6f)); 


	game.get_bear_model()->render(
		game.get_camera3D()->getViewMatrix(),
		game.get_projection3D(),
		&bearMatrix
	);
	
	glm::mat4 lampMatrix = glm::mat4(1.0f);
	lampMatrix = glm::translate(lampMatrix, glm::vec3(2.60f, -0.54f, 2.82f));
	lampMatrix = glm::rotate(lampMatrix, glm::radians(28.82f), glm::vec3(0.0f, 1.0f, 0.0f));
	lampMatrix = glm::scale(lampMatrix, glm::vec3(0.68f));

	//glm::mat4 lampMatrix = positionerObjectHElper(game.get_window(), dt);

	game.get_lamp_model()->render(
		game.get_camera3D()->getViewMatrix(),
		game.get_projection3D(),
		&lampMatrix
	);

	glm::mat4 planeMatrix = glm::mat4(1.0f);
	planeMatrix = glm::translate(planeMatrix, glm::vec3(0.0f, -0.55f, 0.0f));
	//planeMatrix = glm::rotate(planeMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//planeMatrix = glm::scale(planeMatrix, glm::vec3(0.5f));

	game.get_plane_model()->render(
		game.get_camera3D()->getViewMatrix(),
		game.get_projection3D(),
		&planeMatrix
	);
	
}

// ================== InGameState ==========================
void InGameState::enter(Game& game) {
	game.get_enemyManager()->restartSounds();
	//glfwSetInputMode(game.get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void InGameState::update(Game& game, float dt) {
	if (PlayerInput::isKeyJustPressed(game.get_window(), GLFW_KEY_ESCAPE)) {
		game.switch_state(GameStateType::Pause);
	}

	if (game.get_player()->get_life() == 0) {
		game.switch_state(GameStateType::GameOver);
	}

	game.get_player()->update(dt,game.get_window());
	game.get_timer()->update(dt);

	if (game.get_timer()->getMinutes() == 15) {
		game.won = true;
		game.switch_state(GameStateType::GameOver);
	}

	game.get_CastManager()->update(dt);
	game.get_enemyManager()->update(*game.get_player(), dt);
	game.get_bulletManager()->update(dt);

}

void InGameState::exit(Game& game) {
	game.get_enemyManager()->pauseSounds();
	//glfwSetInputMode(game.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InGameState::renderUI(Game& game) {
	// Renderizzo timer e barra della vita
	game.render_baseUI();

	// Stats
	float statsScale = 0.4f;
	float statsMarginRight = 7.5f;
	float statsY = 3.7f;
	float statsLineGap = 0.35f;

	char scoreBuffer[32];
	sprintf_s(scoreBuffer, "Punti: %u", game.get_score());

	char killsBuffer[32];
	sprintf_s(killsBuffer, "Nemici: %u", game.get_enemiesKilled());

	float scoreWidth = game.get_TextRenderer()->GetTextWidth(scoreBuffer, statsScale);
	float killsWidth = game.get_TextRenderer()->GetTextWidth(killsBuffer, statsScale);

	game.get_TextRenderer()->RenderText(
		scoreBuffer,
		statsMarginRight - scoreWidth,
		statsY,
		statsScale,
		{ 1.0f, 1.0f, 1.0f }
	);

	game.get_TextRenderer()->RenderText(
		killsBuffer,
		statsMarginRight - killsWidth,
		statsY - statsLineGap,
		statsScale,
		{ 1.0f, 1.0f, 1.0f }
	);
}

void InGameState::render2d(Game& game) {
	game.render_game2D();
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
	game.render_game2D();

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
	// Renderizzo timer e barra della vita
	game.render_baseUI();

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
	// Renderizzo timer e barra della vita
	game.render_baseUI();

	game.get_SpriteRenderer()->DrawColor(
		{ 0.0f, 0.0f },
		{ 16.0f, 9.0f },
		{ 0.0f, 0.0f, 0.0f, 0.8f }
	);

	std::string title = "Pausa";
	float titleScale = 1.2f;
	float titleWidth = game.get_TextRenderer()->GetTextWidth(title, titleScale);
	game.get_TextRenderer()->RenderText(
		title,
		-titleWidth * 0.5f,
		2.2f,
		titleScale,
		{ 1.0f, 1.0f, 1.0f }
	);

	for (auto& b : game.get_buttons(GameStateType::Pause)) {
		b.render(*game.get_SpriteRenderer(), *game.get_TextRenderer());
	}
}

void PauseGameState::render2d(Game& game) {
	game.render_game2D();
}

// ================== GameOverState =========================
void GameOverState::enter(Game& game) {
	waitingForName = false;
	cursor = 0;
	rankAchieved = -1;
	nameBuffer[0] = nameBuffer[1] = nameBuffer[2] = ' ';
	nameBuffer[3] = '\0';

	rankAchieved = HighscoreManager::getRank(game.get_score());

	if (game.get_score() > 0 && rankAchieved != -1) {
		waitingForName = true;
	}
}

void GameOverState::update(Game& game, float dt) {
	if (waitingForName)
	{
		if (PlayerInput::isKeyJustPressed(game.get_window(), GLFW_KEY_BACKSPACE))
		{
			if (cursor > 0)
			{
				cursor--;
				nameBuffer[cursor] = ' ';
			}
		}

		if (cursor == 3 &&
			PlayerInput::isKeyJustPressed(game.get_window(), GLFW_KEY_ENTER))
		{
			HighscoreManager::saveScore(
				game.get_score(),
				game.get_enemiesKilled(),
				std::string(nameBuffer, 3));

			waitingForName = false;
		}

		return;
	}
	else
	{
		for (auto& b : game.get_buttons(GameStateType::GameOver))
			b.update(game.get_window());
	}
}

void GameOverState::exit(Game& game) {
	game.reset();
}

void GameOverState::renderUI(Game& game) {
	// Renderizzo timer e barra della vita
	game.render_baseUI();

	game.get_SpriteRenderer()->DrawColor(
		{0.0f, 0.0f},
		{16.0f, 9.0f},
		{ 0.0f, 0.0f, 0.0f, 0.8f }
	);

	std::string title = game.won ? "Hai vinto!" : "Game Over";

	float titleScale = 1.2f;
	float titleWidth = game.get_TextRenderer()->GetTextWidth(title, titleScale);
	game.get_TextRenderer()->RenderText(
		title,
		-titleWidth * 0.5f,
		2.2f,
		titleScale,
		{ 1.0f, 1.0f, 1.0f }
	);

	if (waitingForName) {
		char rankMsg[32];
		sprintf_s(rankMsg, "Nuovo record! Posizione #%d", rankAchieved + 1);
		std::string rankStr(rankMsg);
		float rankScale = 0.55f;
		float rankW = game.get_TextRenderer()->GetTextWidth(rankStr, rankScale);
		game.get_TextRenderer()->RenderText(rankStr, -rankW * 0.5f, 1.1f, rankScale, { 1.f, 0.85f, 0.2f });

		std::string prefix = "Inserisci il tuo nome: ";
		char nameDisplay[4] = {
			nameBuffer[0] == ' ' ? '_' : nameBuffer[0],
			nameBuffer[1] == ' ' ? '_' : nameBuffer[1],
			nameBuffer[2] == ' ' ? '_' : nameBuffer[2],
			'\0'
		};
		std::string fullLine = prefix + nameDisplay;
		float lineScale = 0.45f;
		float lineW = game.get_TextRenderer()->GetTextWidth(fullLine, lineScale);
		game.get_TextRenderer()->RenderText(fullLine, -lineW * 0.5f, 0.2f, lineScale, { 1.f, 1.f, 1.f });

		std::string hint = "Premi INVIO per salvare";
		float hintScale = 0.35f;
		float hintW = game.get_TextRenderer()->GetTextWidth(hint, hintScale);
		game.get_TextRenderer()->RenderText(hint, -hintW * 0.5f, -0.5f, hintScale, { 0.6f, 0.6f, 0.6f });
	}
	else {
		for (auto& b : game.get_buttons(GameStateType::GameOver)) {
			b.render(*game.get_SpriteRenderer(), *game.get_TextRenderer());
		}
	}
}

void GameOverState::render2d(Game& game) {
	game.render_game2D();
}

void GameOverState::onChar(unsigned int codepoint)
{
	if (!waitingForName)
		return;

	if (cursor >= 3)
		return;

	if (codepoint >= 'a' && codepoint <= 'z')
		codepoint -= ('a' - 'A');

	if (codepoint >= 'A' && codepoint <= 'Z')
	{
		nameBuffer[cursor++] = static_cast<char>(codepoint);
	}
}

// ================== HighScoreState =========================
void HighScoreState::enter(Game& game) {

}

void HighScoreState::update(Game& game, float dt) {
	for (auto& b : game.get_buttons(GameStateType::HighScore)) {
		b.update(game.get_window());
	}
}

void HighScoreState::exit(Game& game) {

}

void HighScoreState::renderUI(Game& game) {
	game.get_SpriteRenderer()->DrawColor(
		{ 0.0f, 0.0f },
		{ 16.0f, 9.0f },
		{ 0.0f, 0.0f, 0.0f, 0.8f }
	);

	const std::vector<ScoreEntry>& scores = HighscoreManager::get_high_scores();

	// Titolo
	std::string title = "Classifica";
	float titleScale = 1.0f;
	float titleWidth = game.get_TextRenderer()->GetTextWidth(title, titleScale);
	game.get_TextRenderer()->RenderText(
		title,
		-titleWidth * 0.5f,
		2.8f,
		titleScale,
		{ 1.0f, 0.85f, 0.2f }
	);

	// Colonne tabella
	float colRankX = -4.5f;
	float colNameX = -3.5f;
	float colScoreX = -0.5f;
	float colKillsX = 3.5f;

	float headerY = 1.8f;
	float headerScale = 0.7f;
	float rowStartY = 1.2f;
	float rowGap = 0.6f;
	float rowScale = 0.7f;

	// Header
	game.get_TextRenderer()->RenderText("Nome", colNameX, headerY, headerScale, { 0.8f, 0.8f, 0.8f });
	game.get_TextRenderer()->RenderText("Punteggio", colScoreX, headerY, headerScale, { 0.8f, 0.8f, 0.8f });
	game.get_TextRenderer()->RenderText("Nemici", colKillsX, headerY, headerScale, { 0.8f, 0.8f, 0.8f });

	const size_t totalSlots = 5;

	for (size_t i = 0; i < totalSlots; i++) {
		float rowY = rowStartY - rowGap * static_cast<float>(i);

		char rankBuffer[8];
		sprintf_s(rankBuffer, "%d.", static_cast<int>(i + 1));

		if (i < scores.size()) {
			game.get_TextRenderer()->RenderText(rankBuffer, colRankX, rowY, rowScale, { 1.0f, 1.0f, 1.0f });
			game.get_TextRenderer()->RenderText(scores[i].name.c_str(), colNameX, rowY, rowScale, { 1.0f, 1.0f, 1.0f });

			char scoreBuffer[16];
			sprintf_s(scoreBuffer, "%u", scores[i].score);
			game.get_TextRenderer()->RenderText(scoreBuffer, colScoreX, rowY, rowScale, { 1.0f, 1.0f, 1.0f });

			char killsBuffer[16];
			sprintf_s(killsBuffer, "%u", scores[i].enemiesKilled);
			game.get_TextRenderer()->RenderText(killsBuffer, colKillsX, rowY, rowScale, { 1.0f, 1.0f, 1.0f });
		}
		else {
			game.get_TextRenderer()->RenderText(rankBuffer, colRankX, rowY, rowScale, { 0.5f, 0.5f, 0.5f });
			game.get_TextRenderer()->RenderText("---", colNameX, rowY, rowScale, { 0.5f, 0.5f, 0.5f });
			game.get_TextRenderer()->RenderText("---", colScoreX, rowY, rowScale, { 0.5f, 0.5f, 0.5f });
			game.get_TextRenderer()->RenderText("---", colKillsX, rowY, rowScale, { 0.5f, 0.5f, 0.5f });
		}
	}

	for (Button& b : game.get_buttons(GameStateType::HighScore)) {
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
	irrklang::ISoundEngine* audioEngine
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
	audioEngine(audioEngine)
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
	irrklang::ISoundEngine* audioEngine
) {
	static Game game(
		window,
		camera,
		renderer,
		camera3D,
		projection3D,
		audioEngine
	);

	//game.enemyManager->spawn_enemy(EnemyTipe::Mage, 1);+
	game.bulletManager = BulletManager::get_instance();
	game.init_renderers(projection);
	game.init_audio();
	game.init_buttons();
	HighscoreManager::load();
	game.playMusicForState(GameStateType::Title);

	game.castManager->init(&game.figCastRenderer, &game.figAstroCastRenderer, &game.figAstroShadowCastRenderer, &ResourceManager::GetModel("asteroid"));
	EnemyManager::get_instance()->init(audioEngine);

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

void Game::init_audio() {
	musicTracks[GameStateType::Title] = "assets/audio/bgm_title_screen.wav";
	musicTracks[GameStateType::InGame] = "assets/audio/bgm_in_game.wav";
	musicTracks[GameStateType::GameOver] = "assets/audio/bgm_game_over.wav";
}

void Game::init_buttons() {
	SpriteTexture buttonTexture = ResourceManager::GetTexture("button_menu");

	stateButtons[GameStateType::Title] = {
		Button(
			"Inizia",
			glm::vec2(-4.7f, 1.5f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() {spawn_game();  switch_state(GameStateType::InGame); }
		),
		Button(
			"Classifica",
			glm::vec2(-4.7f, -0.5f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() { switch_state(GameStateType::HighScore); }
		),
		Button(
			"Esci",
			glm::vec2(-4.7f, -2.5f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() { glfwSetWindowShouldClose(window, true); }
		)
	};

	stateButtons[GameStateType::Pause] = {
		Button(
			"Riprendi",
			glm::vec2(0.0f, 0.7f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() {  switch_state(GameStateType::InGame); }
		),
		Button(
			"Abbandona",
			glm::vec2(0.0f, -1.3f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() {reset(); switch_state(GameStateType::Title); }
		)
	};

	stateButtons[GameStateType::GameOver] = {
		Button(
			"Riprova",
			glm::vec2(0.0f, 0.7f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() { switch_state(GameStateType::InGame); spawn_game(); }
		),
		Button(
			"Abbandona",
			glm::vec2(0.0f, -1.3f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() { switch_state(GameStateType::Title); }
		)
	};

	stateButtons[GameStateType::HighScore] = {
		Button(
			"Indietro",
			glm::vec2(0.0f, -2.5f),
			glm::vec2(4.0f, 1.5f),
			buttonTexture,
			[this]() { switch_state(GameStateType::Title); }
		),
	};
}

void Game::update(float dt) {
	game_state->update(*this, dt);
	if (chestManager.interact(window, player)) {
		this->switch_state(GameStateType::Looting);
	}
}

void Game::render2d() {
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
	case GameStateType::GameOver:
		game_state = &gameOverState;
		break;
	case GameStateType::HighScore:
		game_state = &highScoreState;
		break;
	}
	game_state->enter(*this);
	playMusicForState(state);
}

void Game::playMusicForState(GameStateType state) {
	auto it = musicTracks.find(state);
	if (it == musicTracks.end()) {
		return;
	}

	if (state == currentMusicState) {
		return;
	}

	if (currentMusic) {
		currentMusic->stop();
		currentMusic->drop();
		currentMusic = nullptr;
	}

	currentMusic = audioEngine->play2D(it->second.c_str(), true, false, true);
	if (currentMusic) {
		currentMusic->setVolume(0.3f);
	}

	currentMusicState = state;
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

std::array<ModelRenderer*, 2> Game::get_chest_part() {
	return { &ResourceManager::GetModel("chest"), &ResourceManager::GetModel("chest_lid") };
}

std::vector<Button>& Game::get_buttons(GameStateType type) {
	return stateButtons[type];
}


ModelRenderer* Game::get_bear_model() {
	return &ResourceManager::GetModel("orso");
}
ModelRenderer* Game::get_lamp_model() {
	return &ResourceManager::GetModel("lamp");
}

ModelRenderer* Game::get_plane_model() {
	return &ResourceManager::GetModel("plane");
}

irrklang::ISoundEngine* Game::get_engine() {
	return audioEngine;
}

void Game::render_game2D() {
	(*renderer).Draw(
		ResourceManager::GetTexture("world"),
		{ 0.0f, 0.0f },              
		{ SCREEN_WIDTH * 2.0f, SCREEN_HEIGHT * 2.0f },
		0.0f,
		camera.getViewMatrix()
	);

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
}

void Game::render_baseUI()
{
	int min = get_timer()->getMinutes();
	int sec = get_timer()->getSeconds();

	char buffer[6];
	sprintf_s(buffer, "%02d:%02d", min, sec);

	float timerScale = 0.7f;
	float timerX = -7.5f;
	float timerY = 3.7f;

	get_TextRenderer()->RenderText(buffer, timerX, timerY, timerScale, { 1.0f, 1.0f, 1.0f });

	// Barra della vita, sotto il timer
	float textWidth = get_TextRenderer()->GetTextWidth(buffer, timerScale);
	float barHeight = 0.2f;
	float gap = 0.15f;

	float barY = timerY - gap - barHeight * 0.5f;

	uint8_t life = get_player()->get_life();
	float lifeRatio = life / 255.0f;

	glm::vec2 barCenter = { timerX + textWidth * 0.5f, barY };
	glm::vec2 barFullSize = { textWidth, barHeight };

	// vita mancante
	get_SpriteRenderer()->DrawColor(
		barCenter,
		barFullSize,
		{ 0.3f, 0.3f, 0.3f, 1.0f }
	);

	// vita attuale
	float redWidth = textWidth * lifeRatio;
	glm::vec2 redCenter = { timerX + redWidth * 0.5f, barY };
	glm::vec2 redSize = { redWidth, barHeight };

	if (redWidth > 0.0f) {
		get_SpriteRenderer()->DrawColor(
			redCenter,
			redSize,
			{ 0.85f, 0.15f, 0.15f, 1.0f }
		);
	}
}

void Game::updateScoreCount(uint16_t score) {
	this->score += score;
	enemiesKilled++;
}

void Game::reset() {
	player.reset();
	enemyManager->reset();
	chestManager.reset();
	bulletManager->reset();
	timer.reset();
	score = 0;
	enemiesKilled = 0;
	won = false;
}

void Game::spawn_game() {
	enemyManager->spawn_enemy(EnemyTipe::Skeleton, 10);
	enemyManager->spawn_enemy(EnemyTipe::Astro, 1);
	chestManager.spawn_chest();
	chestManager.spawn_chest();
}

void Game::onChar(unsigned int c) {
	game_state->onChar(c);
}