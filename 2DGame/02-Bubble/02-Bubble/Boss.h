#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "FallingBamboo.h"
#include <vector>

class Scene;

class Boss
{

public:
	void init(const glm::ivec2& position, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	void setPlayerPosition(const glm::ivec2& pos);

	glm::ivec2 getPosition() { return posEnemy; }

	void setSceneReference(Scene* scene);

	bool isInitialized() const { return sprite != nullptr; }


private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posEnemy, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	float stateTimer;
	bool movingRight;
	float bambooShootTimer;
	float floatDirection;

	Scene* scene = nullptr;

	bool oscillationAnchorSet = false;
	float xAnchor = 0.f;
	bool leafBurstDone = false;

	bool visible = true;
	bool leavesActive = false;
	float leafPhaseTimer = 0.f;
};
