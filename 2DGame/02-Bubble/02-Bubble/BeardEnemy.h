#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


class BeardEnemy
{

public:
	void init(const glm::ivec2& position, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	void setPlayerPosition(const glm::ivec2& pos);

	glm::ivec2 getPosition() { return posEnemy; }

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posEnemy, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	int waitTimer;
	bool pendingDirectionChange;
	bool directionChanged;
	bool facingRightBefore;
	bool facingRight;

};
