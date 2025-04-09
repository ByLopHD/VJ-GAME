#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


class GreenEnemy
{

public:
	void init(const glm::ivec2& position, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	void setPlayerPosition(const glm::ivec2& pos);

	glm::ivec2 getPosition() { return posEnemy; }

	void takeDamage(float amount);
	bool isDead() const;

	bool checkCollision(const std::pair<glm::ivec2, glm::ivec2>& a, const std::pair<glm::ivec2, glm::ivec2>& b);

	pair<glm::ivec2, glm::ivec2> getHitbox();

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

	float health = 0.5f;
	bool dead = false;

};