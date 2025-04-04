#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


class PlayerHealth
{

public:
	void init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram);
	void render();

	void setHeartPosition(int i, glm::vec2 pos);
	int getMaxHearts() const { return maxHearts; }
	void takeDamage(float dmg);


private:
	int maxHearts = 4;     // total de cors
	float health = 4.0f;   // vida en format flotant (per permetre 1/4, 1/2, etc)
	Sprite* sprite;

	Texture spritesheet;
	std::vector<Sprite*> hearts;  // un sprite per cada cor

};
