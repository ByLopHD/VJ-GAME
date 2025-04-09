#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


struct Heart {
	int fragments;  // 0 a 3
};

class PlayerHealth
{

public:
	void init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram);
	void render();

	void setHeartPosition(int i, glm::vec2 pos);
	int getMaxHearts() const { return maxHearts; }
	void takeDamage(float amount);

	void restoreFull();
	void restoreOneHeart();
	void addPumpkin(ShaderProgram& shaderProgram);
	void tryAddExtraHeart(ShaderProgram& shaderProgram);

	int getLives() const { return lives; }

	void loseLife();
	void setLives(int l);

	int getLives() { return lives; };

private:
	//std::vector<Heart> hearts;
	int maxHearts = 4;     // total de cors
	int pumpkinCount = 0;
	float health = 4.0f;   // vida en format flotant (per permetre 1/4, 1/2, etc)
	Sprite* sprite;

	Texture spritesheet;
	std::vector<Sprite*> hearts;  // un sprite per cada cor

	int lives = 3;
};
