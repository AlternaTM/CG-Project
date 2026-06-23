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
	GameOver
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

	Player player;
	GLFWwindow* window;
	Camera& camera;
	
	Camera3D* camera3D;
	const glm::mat4 projection3D;
	std::array<ModelRenderer*, 4> models;

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
	ModelRenderer asteroidModelRenderer = ModelRenderer("assets/models/astro/asteroid.obj", "shaders/asteroid3d/ast3dVertex.glsl", "shaders/asteroid3d/ast3dFrag.glsl");

	irrklang::ISoundEngine* audioEngine;
	uint8_t score = 0;
	uint8_t enemiesKilled = 0;

	SpriteRenderer* renderer;
	EnemyManager* enemyManager;
	Timer timer;
	
	Game(
		GLFWwindow* window,
		Camera& camera,
		SpriteRenderer* renderer,
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		irrklang::ISoundEngine* audioEngine,
		const std::array<ModelRenderer*, 4>& models
	);

	int init_renderers(const glm::mat4& projection);
	void init_buttons();
public: 
	UpgradeUI upgradeUI;


	static void init(
		GLFWwindow* window, 
		Camera& camera, 
		const glm::mat4& projection,
		SpriteRenderer* renderer, 
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		irrklang::ISoundEngine* audioEngine,
		const std::array<ModelRenderer*, 4>& models
	);
	void switch_state(GameStateType new_state);


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


	void reset();
	void spawn_game();

	void render_game2D();

	ChestManager& get_chestManager();

	Camera3D* get_camera3D();
	const glm::mat4 get_projection3D();

	std::array<ModelRenderer*, 2> get_chest_part();
	ModelRenderer* get_bear_model();
	ModelRenderer* get_lamp_model();
	static Game* get_instance();

	void updateScoreCount(uint8_t score);
	uint8_t get_score() { return score; }
	uint8_t get_enemiesKilled() { return enemiesKilled; }
};