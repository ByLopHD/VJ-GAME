#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>

class PowerUp {
public:
	enum class Type { SMALL_HEART, BIG_HEART, PUMPKIN, SPEAR, DEER_SHIRT };

	void init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram, int type);
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void update(int deltaTime);
	void render();
	void setType(int type);
	int getType() const;
	bool checkCollision(const glm::ivec2& playerPos, const glm::ivec2& playerSize);
	bool isActive() const;
	void setActive(bool value);

	glm::ivec2 getPosition() { return posPowerUp; }

private:
	glm::ivec2 tileMapDispl, posPowerUp;
	TileMap* map;
	Texture spritesheet;
	Sprite* sprite;
	Type type;
	bool active;
	bool onGround = false;
	float velocityY = -1.2f;
};

