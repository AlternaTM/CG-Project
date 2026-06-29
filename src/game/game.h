#pragma once
#include "enemy/enemy.h"
#include "../camera/camera3D.h"
#include "../figureRenderer/figureRenderer.h"
#include "../spriteRenderer/spriteRenderer.h"
#include "../chest/chest.h"
#include "modelRenderer/modelRenderer.h"
#include <glm/glm.hpp>
#include <array>
#include "../player/shooting/bullet.h"
#include "../player/upgradeUI/upgrades.h"
#include "textRenderer/textRenderer.h"
#include "timer/timer.h"
#include "button/button.h"
#include <irrKlang/irrKlang.h>

class Game;
class CastManager;
// ================== GAME STATE ==========================

enum GameStateType {
	InGame,
	Pause,
	Looting,
	Title,
	GameOver,
	HighScore
};


class IGameState {
protected:
	GameStateType state_type;
public:
	IGameState(GameStateType type)
		: state_type(type){}
	virtual ~IGameState() = default;
	virtual void enter(Game& game) = 0;
	virtual void update(Game& game, float dt) = 0;
	virtual void exit(Game& game) = 0;
	virtual void render2d(Game& game){}
	virtual void render3d(Game& game, float dt) {}
	virtual void renderUI(Game& game) {}
	virtual void onChar(unsigned int codepoint) {}
	virtual GameStateType get_type() {
		return state_type;
	}
};

class InGameState: public IGameState {
public:
	InGameState() : IGameState(GameStateType::InGame) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void renderUI(Game& game) override;
	virtual void render2d(Game& game) override;
};

class LootingGameState : public IGameState {
public:
	LootingGameState() : IGameState(GameStateType::Looting) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void render2d(Game& game) override;
	virtual void render3d(Game& game, float dt) override;
	virtual void renderUI(Game& game) override;
};

class PauseGameState : public IGameState {
public:
	PauseGameState() : IGameState(GameStateType::Pause) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void renderUI(Game& game) override;
	virtual void render2d(Game& game) override;
};

class TitleGameState : public IGameState {
public:
	TitleGameState() : IGameState(GameStateType::Title) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void render3d(Game& game, float dt) override;
	virtual void renderUI(Game& game) override;
};

class GameOverState : public IGameState {
public:
	GameOverState() : IGameState(GameStateType::GameOver) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void renderUI(Game& game) override;
	virtual void render2d(Game& game) override;
	virtual void onChar(unsigned int codepoint) override;

private:
	bool waitingForName = false;
	char nameBuffer[4] = { ' ', ' ', ' ', '\0' };
	int cursor = 0;
	int rankAchieved = -1;
};

class HighScoreState : public IGameState {
public:
	HighScoreState() : IGameState(GameStateType::HighScore) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void renderUI(Game& game) override;
};

// ================== GAME MANAGER ==========================

class Game {
private:
	static Game* _INSTANCE;
	IGameState* game_state;

	InGameState inGameState;
	LootingGameState lootingState;
	PauseGameState pauseState;
	TitleGameState titleState;
	GameOverState gameOverState;
	HighScoreState highScoreState;

	irrklang::ISound* currentMusic = nullptr;
	GameStateType currentMusicState;
	std::unordered_map<GameStateType, std::string> musicTracks;

	Player player;
	GLFWwindow* window;
	Camera& camera;
	
	Camera3D* camera3D;
	const glm::mat4 projection3D;

	std::unordered_map<GameStateType, std::vector<Button>> stateButtons;

	//EnemyManager* enemyManager;
	CastManager* castManager;
	ChestManager chestManager;
	BulletManager* bulletManager = nullptr;

	//TOOL
	FigRenderer figRectRenderer = FigRenderer("shaders/rect/rectVert.glsl","shaders/rect/rectFrag.glsl");
	FigRenderer figCastRenderer = FigRenderer("shaders/cast/castVert.glsl", "shaders/cast/castFrag.glsl");
	FigRenderer figAstroCastRenderer = FigRenderer("shaders/astroCast/astroVert.glsl","shaders/astroCast/astroFrag.glsl");
	FigRenderer figAstroShadowCastRenderer = FigRenderer("shaders/astroCast/astroShadowVert.glsl", "shaders/astroCast/astroShadowFrag.glsl");
	TextRenderer textRenderer{};


	irrklang::ISoundEngine* audioEngine;
	uint16_t score = 0;
	uint16_t enemiesKilled = 0;

	SpriteRenderer* renderer;
	EnemyManager* enemyManager;
	Timer timer;
	
	Game(
		GLFWwindow* window,
		Camera& camera,
		SpriteRenderer* renderer,
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		irrklang::ISoundEngine* audioEngine
	);

	int init_renderers(const glm::mat4& projection);
	void init_buttons();
	void init_audio();

public: 
	UpgradeUI upgradeUI;

	static void init(
		GLFWwindow* window, 
		Camera& camera, 
		const glm::mat4& projection,
		SpriteRenderer* renderer, 
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		irrklang::ISoundEngine* audioEngine
	);
	void switch_state(GameStateType new_state);
	void playMusicForState(GameStateType state);

	void update(float dt);

	void render2d();
	void render3d(float dt);
	void renderUI();

	Player* get_player();
	Timer* get_timer();
	GLFWwindow* get_window();
	CastManager* get_CastManager();
	EnemyManager* get_enemyManager();
	BulletManager* get_bulletManager();
	SpriteRenderer* get_SpriteRenderer();
	TextRenderer* get_TextRenderer();

	irrklang::ISoundEngine* get_engine();

	std::vector<Button>& get_buttons(GameStateType type);
	bool won = false;
	void onChar(unsigned int codepoint);

	void reset();
	void spawn_game();

	void render_game2D();

	ChestManager& get_chestManager();

	Camera3D* get_camera3D();
	const glm::mat4 get_projection3D();

	std::array<ModelRenderer*, 2> get_chest_part();
	ModelRenderer* get_bear_model();
	ModelRenderer* get_lamp_model();
	ModelRenderer* get_plane_model();
	static Game* get_instance();

	void updateScoreCount(uint16_t score);
	uint16_t get_score() { return score; }
	uint16_t get_enemiesKilled() { return enemiesKilled; }
};


inline glm::mat4 positionerObjectHElper(GLFWwindow* window, float dt)
{
	static glm::vec3 pos(0.0f);
	static float rotY = 0.0f;
	static float scale = 1.0f;

	float moveSpeed = 2.0f * dt;
	float rotSpeed = 90.0f * dt;
	float scaleSpeed = 1.0f * dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos.z -= moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos.z += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos.x -= moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos.x += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) pos.y += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) pos.y -= moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotY -= rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotY += rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) scale += scaleSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) scale = std::max(0.01f, scale - scaleSpeed);

	static bool pWasPressed = false;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pWasPressed) {
		std::cout << "[debugMatrix] pos(" << pos.x << ", " << pos.y << ", " << pos.z << ") "
			<< "rotY=" << rotY << " scale=" << scale << std::endl;
		pWasPressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		pWasPressed = false;
	}

	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, pos);
	m = glm::rotate(m, glm::radians(rotY), glm::vec3(0, 1, 0));
	m = glm::scale(m, glm::vec3(scale));
	return m;
}