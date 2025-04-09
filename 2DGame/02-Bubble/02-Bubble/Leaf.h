#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


class Leaf
{

public:
	void init(const glm::ivec2& position, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	void setPlayerPosition(const glm::ivec2& pos);

	glm::ivec2 getPosition() { return posEnemy; }

	void updatePosition(float radius);

	void setAngle(float angle);  // 🔥 Defineix l'angle únic per cada fulla

	bool isActive() const { return active; }
	void setActive(bool b) { active = b; }

	void setInitialPosition(const glm::vec2& pos);


private:
	glm::ivec2 tileMapDispl, posEnemy, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	float timeAlive = 0.f;
	float maxRadius = 3 * 16.f; // màxim radi: 3 tileSize
	glm::vec2 center;
	bool active;
	float angle;
	float timeElapsed;
};
