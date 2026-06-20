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

class Game;
class CastManager;
// ================== GAME STATE ==========================

enum GameStateType {
	InGame,
	Pause,
	Looting
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
};

class LootingGameState : public IGameState {
public:
	LootingGameState() : IGameState(GameStateType::Looting) {};
	virtual void enter(Game& game) override;
	virtual void update(Game& game, float dt) override;
	virtual void exit(Game& game) override;
	virtual void render3d(Game& game, float dt) override;
};

// ================== GAME MANAGER ==========================





class Game {
private:
	static Game* _INSTANCE;
	IGameState* game_state;

	InGameState inGameState;
	LootingGameState lootingState;

	Player player;
	GLFWwindow* window;
	Camera& camera;
	
	Camera3D* camera3D;
	const glm::mat4 projection3D;
	std::array<ModelRenderer*, 2> chest_part;

	//EnemyManager* enemyManager;
	CastManager* castManager;
	ChestManager chestManager;
	BulletManager* bulletManager = nullptr;

	//TOOL
	FigRenderer figRectRenderer = FigRenderer("shaders/rect/rectVert.glsl","shaders/rect/rectFrag.glsl");
	FigRenderer figCastRenderer = FigRenderer("shaders/cast/castVert.glsl", "shaders/cast/castFrag.glsl");
	FigRenderer figAstroCastRenderer = FigRenderer("shaders/astroCast/astroVert.glsl","shaders/astroCast/astroFrag.glsl");
	FigRenderer figAstroShadowCastRenderer = FigRenderer("shaders/astroCast/astroShadowVert.glsl", "shaders/astroCast/astroShadowFrag.glsl");
	TextRenderer textRenderer{ 1920, 1080 };
	ModelRenderer asteroidModelRenderer = ModelRenderer("assets/models/astro/asteroid.obj", "shaders/asteroid3d/ast3dVertex.glsl", "shaders/asteroid3d/ast3dFrag.glsl");

	SpriteRenderer* renderer;
	EnemyManager* enemyManager;
	Timer timer;
	


	Game(
		GLFWwindow* window,
		Camera& camera,
		SpriteRenderer* renderer,
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		const std::array<ModelRenderer*, 2>& chest_part
	);

	int init_renderers(const glm::mat4& projection);
public: 
	UpgradeUI upgradeUI;


	static void init(
		GLFWwindow* window, 
		Camera& camera, 
		const glm::mat4& projection,
		SpriteRenderer* renderer, 
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		const std::array<ModelRenderer*, 2>& chest_part
	);
	void switch_state(GameStateType new_state);


	void update(float dt);

	void render2d();
	void render3d(float dt);
	void renderUI();

	Player* get_player();
	GLFWwindow* get_window();
	CastManager* get_CastManager();
	EnemyManager* get_enemyManager();
	BulletManager* get_bulletManager();

	ChestManager& get_chestManager();

	Camera3D* get_camera3D();
	const glm::mat4 get_projection3D();

	std::array<ModelRenderer*, 2>& get_chest_part();
	static Game* get_instance();
};