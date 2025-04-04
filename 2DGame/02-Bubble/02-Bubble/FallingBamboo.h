#pragma once
#include "Sprite.h"
#include "TileMap.h"

class FallingBamboo
{
public:
    void init(const glm::ivec2& position, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPlayerPosition(const glm::ivec2& playerPos);

    bool checkCollisionWithPlayer() const;
    bool isActive() const;

    glm::ivec2 getPosition() { return posBamboo; }

private:
    glm::ivec2 posBamboo;
    glm::ivec2 size = glm::ivec2(16, 32); // tama�o de bamboo
    glm::ivec2 playerPos;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

    float speed = 3.0f;
    bool active = true;
};

/*
#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


class FallingBamboo
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
^*/