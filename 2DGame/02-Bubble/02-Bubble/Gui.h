#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <vector>


class Gui
{

public:
	void init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram);
	void setLives(int l);
	void setPosition(const glm::ivec2& pos);
	void render();

	void setGuiPosition(int i, glm::vec2 pos);


private:
	//std::vector<Heart> hearts;
	Sprite* arma;
	std::vector<Sprite*> linternes;
	std::vector<bool> linternaVisible;

	Texture spritesheet;

	int lives = 3;
	int num_lint = 2;
};
