#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>

class BossHealth
{

public:
	void init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram);
	void render();

	void setHeartPosition(int i, glm::vec2 pos);
	int getMaxHearts() const { return maxHearts; }
	void takeDamage(float amount);

	bool isDead() const;

private:
	//std::vector<Heart> hearts;
	int maxHearts = 6;     // total de cors
	int pumpkinCount = 0;
	float health = 6.0f;   // vida en format flotant (per permetre 1/4, 1/2, etc)
	Sprite* sprite;

	Texture spritesheet;
	std::vector<Sprite*> hearts;  // un sprite per cada cor
};
#pragma once
