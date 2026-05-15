#pragma once
#include "enemy/enemy.h"
#include "../camera/camera3D.h"
#include "../figureRenderer/figureRenderer.h"
#include "../spriteRenderer/spriteRenderer.h"
#include "../chest/chest.h"
#include "../modelRenderer/modelRenderer.h"
#include <glm/glm.hpp>
#include <array>
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

	//TOOL
	FigRenderer* figRectRenderer;
	FigRenderer* figCastRenderer;
	SpriteRenderer* renderer;
	EnemyManager* enemyManager;



	Game(
		GLFWwindow* window,
		Camera& camera,
		SpriteRenderer* renderer,
		FigRenderer* figRectRenderer,
		FigRenderer* figCastRenderer,
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		const std::array<ModelRenderer*, 2>& chest_part
	);
public: 
	
	static void init(
		GLFWwindow* window, 
		Camera& camera, 
		SpriteRenderer* renderer, 
		FigRenderer* figRectRenderer, 
		FigRenderer* figCastRenderer,
		Camera3D* camera3D,
		const glm::mat4 projection3D,
		const std::array<ModelRenderer*, 2>& chest_part
	);
	void switch_state(GameStateType new_state);

	void update(float dt);

	void render2d();
	void render3d(float dt);

	Player* get_player();
	GLFWwindow* get_window();
	CastManager* get_CastManager();
	EnemyManager* get_enemyManager();

	Camera3D* get_camera3D();
	const glm::mat4 get_projection3D();

	std::array<ModelRenderer*, 2>& get_chest_part();
	static Game* get_instance();
};