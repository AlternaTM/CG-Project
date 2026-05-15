#pragma once
#include "enemy/enemy.h"

#include "../figureRenderer/figureRenderer.h"

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
	//EnemyManager* enemyManager;
	CastManager* castManager;
	ChestManager chestManager;

	//TOOL
	FigRenderer* figRectRenderer;
	FigRenderer* figCastRenderer;
	SpriteRenderer* renderer;
	EnemyManager* enemyManager;


	Game(GLFWwindow* window, Camera& camera, SpriteRenderer* renderer, FigRenderer* figRectRenderer, FigRenderer* figCastRenderer);
public: 
	
	static void init(GLFWwindow* window, Camera& camera, SpriteRenderer* renderer, FigRenderer* figRectRenderer, FigRenderer* figCastRenderer);
	void switch_state(GameStateType new_state);

	void update(float dt);

	void render();

	Player* get_player();
	GLFWwindow* get_window();
	CastManager* get_CastManager();
	EnemyManager* get_enemyManager();


	static Game* get_instance();
};